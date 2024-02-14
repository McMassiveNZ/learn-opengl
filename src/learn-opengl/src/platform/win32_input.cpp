#include "pch.h"
#include "input.h"
#include "wmcv_log/wmcv_log.h"

namespace wmcv
{
	static bool CreateRawInput()
	{
		std::array<RAWINPUTDEVICE, 2> devices =
		{
			RAWINPUTDEVICE 
			{  
				// RIM_TYPEKEYBOARD
				.usUsagePage = HID_USAGE_PAGE_GENERIC, 
				.usUsage = HID_USAGE_GENERIC_KEYBOARD, 
				.dwFlags = 0, 
				.hwndTarget = nullptr 
			},
			RAWINPUTDEVICE
			{	  
				// RIM_TYPEMOUSE
				.usUsagePage = HID_USAGE_PAGE_GENERIC, 
				.usUsage = HID_USAGE_GENERIC_MOUSE, 
				.dwFlags = 0, 
				.hwndTarget = nullptr
			}
		};

		const UINT size = static_cast<UINT>(devices.size());
		return ::RegisterRawInputDevices(devices.data(), size, sizeof(RAWINPUTDEVICE)) == TRUE;
	}

	struct CursorState
	{
		POINT pos;
		bool hidden;
	};

	struct Win32InputImpl final : public Input
	{
		Win32InputImpl()
		{
			HideCursor();
			CreateRawInput();
		}

		virtual auto FlushEvents() -> InputStream;
		virtual auto HandleWindowMessage(uintptr_t, uint64_t, uintptr_t, uintptr_t) -> bool;
		virtual void ShowCursor();
		virtual void HideCursor();
		virtual bool IsCursorHidden() const;

		InputStream events;
		ButtonsState state;
		CursorState cursor;
	};


	//https://blog.molecular-matters.com/2011/09/05/properly-handling-keyboard-input/
	static void HandleRawKeyboard(const RAWKEYBOARD& rawKB, [[maybe_unused]] Win32InputImpl& input)
	{
		UINT virtualKey = rawKB.VKey;
		UINT scanCode = rawKB.MakeCode;

		if (virtualKey == 255)
		{
			// discard "fake keys" which are part of an escaped sequence
			return;
		}
		else if (virtualKey == VK_SHIFT)
		{
			// correct left-hand / right-hand SHIFT
			virtualKey = MapVirtualKey(scanCode, MAPVK_VSC_TO_VK_EX);
		}
		else if (virtualKey == VK_NUMLOCK)
		{
			// correct PAUSE/BREAK and NUM LOCK silliness, and set the extended bit
			scanCode = (MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC) | 0x100);
		}

		// e0 and e1 are escape sequences used for certain special keys, such as PRINT and PAUSE/BREAK.
		// see http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
		const bool isE0 = ((rawKB.Flags & RI_KEY_E0) != 0);
		const bool isE1 = ((rawKB.Flags & RI_KEY_E1) != 0);

		if (isE1)
		{
			// for escaped sequences, turn the virtual key into the correct scan code using MapVirtualKey.
			// however, MapVirtualKey is unable to map VK_PAUSE (this is a known bug), hence we map that by hand.
			if (virtualKey == VK_PAUSE)
				scanCode = 0x45;
			else
				scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);
		}

		InputKey key = static_cast<InputKey>(virtualKey);

		switch (virtualKey)
		{
			// right-hand CONTROL and ALT have their e0 bit set
			case VK_CONTROL:
				if (isE0)
					key = InputKey::RIGHT_CTRL;
				else
					key = InputKey::LEFT_CTRL;
				break;

			case VK_MENU:
				if (isE0)
					key = InputKey::RIGHT_ALT;
				else
					key = InputKey::LEFT_ALT;
				break;

			// NUMPAD ENTER has its e0 bit set
			case VK_RETURN:
				if (isE0)
					key = InputKey::NUMPAD_ENTER;
				break;

			// the standard INSERT, DELETE, HOME, END, PRIOR and NEXT keys will always have their e0 bit set, but the
			// corresponding keys on the NUMPAD will not.
			case VK_INSERT:
				if (!isE0)
					key = InputKey::NUMPAD0;
				break;

			case VK_DELETE:
				if (!isE0)
					key = InputKey::NUMPAD_DECIMAL;
				break;

			case VK_HOME:
				if (!isE0)
					key = InputKey::NUMPAD7;
				break;

			case VK_END:
				if (!isE0)
					key = InputKey::NUMPAD1;
				break;

			case VK_PRIOR:
				if (!isE0)
					key = InputKey::NUMPAD9;
				break;

			case VK_NEXT:
				if (!isE0)
					key = InputKey::NUMPAD3;
				break;

			// the standard arrow keys will always have their e0 bit set, but the
			// corresponding keys on the NUMPAD will not.
			case VK_LEFT:
				if (!isE0)
					key = InputKey::NUMPAD4;
				break;

			case VK_RIGHT:
				if (!isE0)
					key = InputKey::NUMPAD6;
				break;

			case VK_UP:
				if (!isE0)
					key = InputKey::NUMPAD8;
				break;

			case VK_DOWN:
				if (!isE0)
					key = InputKey::NUMPAD2;
				break;

			// NUMPAD 5 doesn't have its e0 bit set
			case VK_CLEAR:
				if (!isE0)
					key = InputKey::NUMPAD5;
				break;
		}

		// a key can either produce a "make" or "break" scancode. this is used to differentiate between down-presses and releases
		// see http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
		const bool down = ((rawKB.Flags & RI_KEY_BREAK) == 0);
		if (std::exchange(input.state[key], down) != down)
		{
			input.events.emplace_back(key, down ? InputEventType::KeyDown : InputEventType::KeyUp);
		}

#if false //in case we want to handle typing

		// getting a human-readable string
		UINT key = (scanCode << 16) | (isE0 << 24);
		char buffer[512] = {};
		GetKeyNameText(static_cast<LONG>(key), buffer, sizeof(buffer));
#endif
	}

	void HandleRawMouse(const RAWMOUSE& raw_mouse, HWND window, Win32InputImpl& input)
	{
		float x = static_cast<float>(raw_mouse.lLastX);
		float y = static_cast<float>(raw_mouse.lLastY);
		if ((raw_mouse.usFlags & MOUSE_MOVE_ABSOLUTE) == MOUSE_MOVE_ABSOLUTE)
		{
			const bool virtualDesktop = (raw_mouse.usFlags & MOUSE_VIRTUAL_DESKTOP) == MOUSE_VIRTUAL_DESKTOP;
			const int32_t width = GetSystemMetrics(virtualDesktop ? SM_CXVIRTUALSCREEN : SM_CXSCREEN);
			const int32_t height = GetSystemMetrics(virtualDesktop ? SM_CYVIRTUALSCREEN : SM_CYSCREEN);

			static glm::vec2 MAX = glm::vec2( std::numeric_limits<float>::max(), std::numeric_limits<float>::max() );
			static glm::vec2 prev = MAX;
			const glm::vec2 curr{(x / 65535.0f) * width, (y / 65535.0f) * height};

			if (prev != MAX)
			{
				x = curr.x - prev.x;
				y = curr.y - prev.y;
			}
			prev = curr;
		}

		if (x != 0.f || y != 0.f)
		{
			const DWORD pos = ::GetMessagePos();
			POINT cursor = {
				.x = GET_X_LPARAM(pos),
				.y = GET_Y_LPARAM(pos)
			};

			::ScreenToClient(window, &cursor);

			input.events.emplace_back(cursor.x, cursor.y, static_cast<int32_t>(x), static_cast<int32_t>(y));
		}

		if (raw_mouse.ulButtons & RI_MOUSE_WHEEL)
		{
			const auto wheel_delta = static_cast<SHORT>(raw_mouse.usButtonData) / WHEEL_DELTA;
			input.events.emplace_back(wheel_delta);
		}

		const auto handleButtonState = [&](LONG action, InputKey key, bool down) 
		{
			if (raw_mouse.ulButtons & action)
			{
				if (std::exchange(input.state[key], down) != down)
				{
					const auto eventType = down ? InputEventType::MouseDown : InputEventType::MouseUp;
					input.events.emplace_back(key, eventType);

					wmcv::LogMessage("Mouse Button {}\n", down ? "Down" : "Up");
				}
			}
		};

		handleButtonState(RI_MOUSE_BUTTON_1_DOWN, InputKey::LBUTTON, true);
		handleButtonState(RI_MOUSE_BUTTON_1_UP, InputKey::LBUTTON, false);
		handleButtonState(RI_MOUSE_BUTTON_2_DOWN, InputKey::RBUTTON, true);
		handleButtonState(RI_MOUSE_BUTTON_2_UP, InputKey::RBUTTON, false);
		handleButtonState(RI_MOUSE_BUTTON_3_DOWN, InputKey::MBUTTON, true);
		handleButtonState(RI_MOUSE_BUTTON_3_UP, InputKey::MBUTTON, false);
		handleButtonState(RI_MOUSE_BUTTON_4_DOWN, InputKey::MOUSE4, true);
		handleButtonState(RI_MOUSE_BUTTON_4_UP, InputKey::MOUSE4, false);
		handleButtonState(RI_MOUSE_BUTTON_5_DOWN, InputKey::MOUSE5, true);
		handleButtonState(RI_MOUSE_BUTTON_5_UP, InputKey::MOUSE5, false);
	}

	auto Win32InputImpl::HandleWindowMessage(uintptr_t window, uint64_t message, [[maybe_unused]] uintptr_t wparam, uintptr_t lparam) -> bool
	{
		if (message == WM_INPUT)
		{
			UINT dwSize = {};
			HRAWINPUT hraw = reinterpret_cast<HRAWINPUT>(lparam);
			GetRawInputData(hraw, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
			if (auto lpb = std::make_unique<BYTE[]>(dwSize); lpb)
			{
				if (GetRawInputData(hraw, RID_INPUT, lpb.get(), &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
				{
					wmcv::LogMessage("GetRawInputData does not return correct size !\n");
					return false;
				}

				static_assert(std::is_standard_layout_v<RAWINPUT>);

				RAWINPUT raw = {};
				std::memcpy(&raw, lpb.get(), dwSize);

				if (raw.header.dwType == RIM_TYPEKEYBOARD)
				{
					HandleRawKeyboard(raw.data.keyboard, *this);
					return true;
				}
				else if (raw.header.dwType == RIM_TYPEMOUSE)
				{
					HandleRawMouse(raw.data.mouse, reinterpret_cast<HWND>(window), *this);
					return true;
				}
			}
		}
		else if (message == WM_SETCURSOR)
		{
			HWND hWnd = reinterpret_cast<HWND>(window);

			if (cursor.hidden)
			{
				RECT rect = {};
				WINDOWPLACEMENT placement = {};
				GetWindowRect(hWnd, &rect);
				GetWindowPlacement(hWnd, &placement);

				POINT pos = {
					.x = ((rect.right - rect.left) / 2) + placement.rcNormalPosition.left,
					.y = ((rect.bottom - rect.top) / 2) + placement.rcNormalPosition.top
				};

				SetCursorPos(pos.x, pos.y);
				SetCursor(LoadCursor(NULL, IDC_CROSS));
			}
			else
			{
				SetCursor(LoadCursor(NULL, IDC_ARROW));
			}

			return true;
		}

		return false;
	}

	void Win32InputImpl::ShowCursor()
	{
		cursor.hidden = false;
		SetCursorPos(cursor.pos.x, cursor.pos.y);
	}

	void Win32InputImpl::HideCursor()
	{
		cursor.hidden = true;
		GetCursorPos(&cursor.pos);
	}

	auto Win32InputImpl::IsCursorHidden() const -> bool
	{
		return cursor.hidden;
	}

	auto Win32InputImpl::FlushEvents() -> InputStream
	{
		return std::exchange(events, {});
	}

	std::unique_ptr<Input> Input::Create()
	{
		return std::make_unique<Win32InputImpl>();
	}
}

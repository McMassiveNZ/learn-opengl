#include "pch.h"
#include "window.h"
#include "application.h"

namespace wmcv
{

struct Win32WindowImpl final : public IWindow
{
	void PumpMessages();
	bool IsOpen() const;
	void* GetNativeHandle() const;
	void PushSink(IWindowMessageSink* sink);

	std::vector<IWindowMessageSink*> m_sinks = {};
	HINSTANCE m_hInstance = nullptr;
	HWND m_hWnd = nullptr;
	bool m_close = true;
};

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CLOSE)
	{
		PostQuitMessage(0);
		return 0;
	}

	using window_span = std::span<IWindowMessageSink*>;
	window_span sinks = [=]
	{
		wmcv::Win32WindowImpl* impl = nullptr;
		if (message == WM_CREATE || message == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			impl = reinterpret_cast<wmcv::Win32WindowImpl*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)impl);
		}
		else
		{
			impl = reinterpret_cast<wmcv::Win32WindowImpl*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		}

		return impl ? window_span{impl->m_sinks} : window_span{};
	}();

	for (auto* sink : sinks)
	{
		if (sink->HandleWindowMessage(reinterpret_cast<uintptr_t>(hWnd), message, wParam, lParam))
		{
			return 0;
		}
	}

	#if 0
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			const auto keycode = static_cast<wmcv::InputKey>(wParam);
			app->processButtonPressEvent(keycode, wmcv::InputEventType::KeyDown);
		}
		break;

		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			const auto keycode = static_cast<wmcv::InputKey>(wParam);
			app->processButtonPressEvent(keycode, wmcv::InputEventType::KeyUp);
		}
		break;

		case WM_LBUTTONDOWN:
		{
			app->processButtonPressEvent(wmcv::InputKey::LBUTTON, wmcv::InputEventType::MouseDown);
		}
		break;

		case WM_LBUTTONUP:
		{
			app->processButtonPressEvent(wmcv::InputKey::LBUTTON, wmcv::InputEventType::MouseUp);
		}
		break;

		case WM_RBUTTONDOWN:
		{
			app->processButtonPressEvent(wmcv::InputKey::RBUTTON, wmcv::InputEventType::MouseDown);
		}
		break;

		case WM_RBUTTONUP:
		{
			app->processButtonPressEvent(wmcv::InputKey::RBUTTON, wmcv::InputEventType::MouseUp);
		}
		break;

		case WM_MBUTTONDOWN:
		{
			app->processButtonPressEvent(wmcv::InputKey::MBUTTON, wmcv::InputEventType::MouseDown);
		}
		break;

		case WM_MBUTTONUP:
		{
			app->processButtonPressEvent(wmcv::InputKey::MBUTTON, wmcv::InputEventType::MouseUp);
		}
		break;

		case WM_MOUSEMOVE:
		{
			const int32_t x = GET_X_LPARAM(lParam);
			const int32_t y = GET_Y_LPARAM(lParam);
			app->processMouseMoveEvent(x, y);
		}
		break;
	switch (message)
	{


		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
#else
	return DefWindowProc(hWnd, message, wParam, lParam);
	#endif
}


void Win32WindowImpl::PumpMessages()
{
	MSG message = {};
	if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE) != FALSE)
	{
		if ( message.message == WM_QUIT )	
		{
			m_close = true;
		}

		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

bool Win32WindowImpl::IsOpen() const
{
	return m_close == false;
}

void* Win32WindowImpl::GetNativeHandle() const
{
	return static_cast<void*>(m_hWnd);
}

void Win32WindowImpl::PushSink(IWindowMessageSink* sink)
{
	m_sinks.push_back(sink);
}

std::unique_ptr<IWindow> IWindow::Create(const wmcv::WindowCreateParams& params)
{
	const char className[] = "Win32WindowImpl";
	HINSTANCE hInstance = GetModuleHandle(NULL);

	auto result = std::make_unique<Win32WindowImpl>();

	WNDCLASSEX wc = {};

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = className;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	if (RegisterClassEx(&wc) == 0)
	{
		MessageBox(nullptr, "Call to RegisterClass failed", "Fatal Error", MB_OK);
		return result;
	}

	HWND window = CreateWindowEx(
		0,
		className,
		params.name,
		WS_OVERLAPPEDWINDOW,
		params.x, params.y, params.width, params.height,
		nullptr,
		nullptr,
		hInstance,
		result.get());

	if (window == nullptr)
	{
		MessageBox(nullptr, "Call to CreateWindow failed", "Fatal Error", MB_OK);
		return result;
	}

	ShowWindow(window, SW_SHOW);

	result->m_hInstance = hInstance;
	result->m_hWnd = window;
	result->m_close = false;

	return result;
}
} // namespace ogl_starter

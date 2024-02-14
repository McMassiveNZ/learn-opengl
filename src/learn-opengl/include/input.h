#ifndef WMCV_INPUT_H_INCLUDED
#define WMCV_INPUT_H_INCLUDED

#include "window.h"

namespace wmcv
{
	enum class InputEventType : uint8_t
	{
		KeyDown,
		KeyUp,
		MouseDown,
		MouseUp,
		MouseMove,
		MouseWheel
	};

	enum class InputKey : uint8_t
	{
		INVALID = 0x0,

		LBUTTON = 0x01,
		RBUTTON = 0x02,
		CANCEL = 0x03,
		MBUTTON = 0x04,
		BACK = 0x08,
		TAB = 0x09,
		CLEAR = 0x0C,
		RETURN = 0x0D,
		PAUSE = 0x13,
		CAPITAL = 0x14,
		ESCAPE = 0x1B,
		CONVERT = 0x1C,
		NONCONVERT = 0x1D,
		ACCEPT = 0x1E,
		MODECHANGE = 0x1F,
		SPACE = 0x20,
		PRIOR = 0x21,
		NEXT = 0x22,
		END = 0x23,
		HOME = 0x24,
		LEFT = 0x25,
		UP = 0x26,
		RIGHT = 0x27,
		DOWN = 0x28,
		SELECT = 0x29,
		PRINT = 0x2A,
		SNAPSHOT = 0x2C,
		INSERT = 0x2D,
		DEL = 0x2E,
		HELP = 0x2F,
		_0 = 0x30,
		_1 = 0x31,
		_2 = 0x32,
		_3 = 0x33,
		_4 = 0x34,
		_5 = 0x35,
		_6 = 0x36,
		_7 = 0x37,
		_8 = 0x38,
		_9 = 0x39,
		A = 0x41,
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		J = 0x4A,
		K = 0x4B,
		L = 0x4C,
		M = 0x4D,
		N = 0x4E,
		O = 0x4F,
		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5A,
		LWIN = 0x5B,
		RWIN = 0x5C,
		APPS = 0x5D,
		SLEEP = 0x5F,
		NUMPAD0 = 0x60,
		NUMPAD1 = 0x61,
		NUMPAD2 = 0x62,
		NUMPAD3 = 0x63,
		NUMPAD4 = 0x64,
		NUMPAD5 = 0x65,
		NUMPAD6 = 0x66,
		NUMPAD7 = 0x67,
		NUMPAD8 = 0x68,
		NUMPAD9 = 0x69,
		NUMPAD_ENTER = 0x7,
		NUMPAD_MULTIPLY = 0x6A,
		NUMPAD_ADD = 0x6B,
		NUMPAD_SEPARATOR = 0x6C,
		NUMPAD_SUBTRACT = 0x6D,
		NUMPAD_DECIMAL = 0x6E,
		NUMPAD_DIVIDE = 0x6F,
		F1 = 0x70,
		F2 = 0x71,
		F3 = 0x72,
		F4 = 0x73,
		F5 = 0x74,
		F6 = 0x75,
		F7 = 0x76,
		F8 = 0x77,
		F9 = 0x78,
		F10 = 0x79,
		F11 = 0x7A,
		F12 = 0x7B,
		F13 = 0x7C,
		F14 = 0x7D,
		F15 = 0x7E,
		F16 = 0x7F,
		F17 = 0x80,
		F18 = 0x81,
		F19 = 0x82,
		F20 = 0x83,
		F21 = 0x84,
		F22 = 0x85,
		F23 = 0x86,
		F24 = 0x87,
		NAVIGATION_VIEW = 0x88,
		NAVIGATION_MENU = 0x89,
		NAVIGATION_UP = 0x8A,
		NAVIGATION_DOWN = 0x8B,
		NAVIGATION_LEFT = 0x8C,
		NAVIGATION_RIGHT = 0x8D,
		NAVIGATION_ACCEPT = 0x8E,
		NAVIGATION_CANCEL = 0x8F,
		NUMLOCK = 0x90,
		SCROLL = 0x91,
		LEFT_SHIFT = 0xA0,
		RIGHT_SHIFT = 0xA1,
		LEFT_CTRL = 0xA2,
		RIGHT_CTRL = 0xA3,
		LEFT_ALT = 0xA4,
		RIGHT_ALT = 0xA5,
		MOUSE0 = 0xB0,
		MOUSE1 = 0xB1,
		MOUSE2 = 0xB2,
		MOUSE3 = 0xB3,
		MOUSE4 = 0xB4,
		MOUSE5 = 0xB5,
		MOUSE6 = 0xB6,
		MOUSE7 = 0xB7,
		MOUSE8 = 0xB8,
		MOUSE9 = 0xB9,
		MOUSE10 = 0xBA,
		MOUSE11 = 0xBB,
		MOUSE12 = 0xBC,
		MOUSE13 = 0xBD,
		MOUSE14 = 0xBE,
		MOUSE15 = 0xBF,
		MINUS = 0xC0,
		EQUAL = 0xC1,
		LBRACKET = 0xC2,
		RBRACKET = 0xC3,
		SEMICOLON = 0xC4,
		APOSTROPHE = 0xC5,
		COMMA = 0xC6,
		PERIOD = 0xC7,
		SLASH = 0xC8,
		BACKSLASH = 0xC9,
		GRAVE = 0xCA,
		PAD_A = 0xD0,
		PAD_CROSS = 0xD0,
		PAD_B = 0xD1,
		PAD_CIRCLE = 0xD1,
		PAD_X = 0xD2,
		PAD_SQUARE = 0xD2,
		PAD_Y = 0xD3,
		PAD_TRIANGLE = 0xD3,
		PAD_START = 0xD4,
		PAD_BACK = 0xD5,
		PAD_DIGITAL_UP = 0xD6,
		PAD_DIGITAL_DOWN = 0xD7,
		PAD_DIGITAL_LEFT = 0xD8,
		PAD_DIGITAL_RIGHT = 0xD9,
		PAD_LEFT_THUMB = 0xDA,
		PAD_RIGHT_THUMB = 0xDB,
		PAD_LEFT_SHOULDER = 0xDC,
		PAD_RIGHT_SHOULDER = 0xDD,
		PAD_LEFT_TRIGGER = 0xDE,
		PAD_RIGHT_TRIGGER = 0xDF,

		MAX = 0xFF,
	};

	constexpr size_t NumKeys = static_cast<size_t>(InputKey::MAX);

	class ButtonsState
	{
		std::array<bool, NumKeys> state = {};

	public:
		constexpr bool operator[](size_t idx) const { return state[idx]; }
		constexpr bool& operator[](size_t idx) { return state[idx]; }
		constexpr bool operator[](InputKey key) const { return state[static_cast<size_t>(key)]; }
		constexpr bool& operator[](InputKey key) { return state[static_cast<size_t>(key)]; }

		decltype(state)::iterator begin() { return state.begin(); }
		decltype(state)::const_iterator cbegin() { return state.cbegin(); }
		decltype(state)::iterator end() { return state.end(); }
		decltype(state)::const_iterator cend() { return state.cend(); }
	};
	
	struct InputEvent
	{
		InputEvent(InputKey k, InputEventType t)
			: key(k)
			, type(t)
			, abs_x(0)
			, abs_y(0)
			, rel_x(0)
			, rel_y(0)
			, wheel(0)
		{
		}
		
		InputEvent(int32_t absx, int32_t absy, int32_t relx, int32_t rely)
			: key(InputKey::INVALID)
			, type(InputEventType::MouseMove)
			, abs_x(absx)
			, abs_y(absy)
			, rel_x(relx)
			, rel_y(rely)
			, wheel(0)
		{}

		InputEvent(int32_t wheelDelta)
			: key(InputKey::INVALID)
			, type(InputEventType::MouseWheel)
			, abs_x(0)
			, abs_y(0)
			, rel_x(0)
			, rel_y(0)
			, wheel(wheelDelta)
		{}

		InputKey key;
		InputEventType type;
		int32_t abs_x;
		int32_t abs_y;
		int32_t rel_x;
		int32_t rel_y;
		int32_t wheel;
	};

	using InputStream = std::vector<InputEvent>;

	class Input : public IWindowMessageSink 
	{
	public:
		virtual ~Input() = default;
		virtual auto FlushEvents() -> InputStream = 0;
		virtual void ShowCursor() = 0;
		virtual void HideCursor() = 0;
		virtual bool IsCursorHidden() const = 0;

		static std::unique_ptr<Input> Create();
	};

	using InputPtr = std::unique_ptr<Input>;
}

#endif //WMCV_INPUT_H_INCLUDED
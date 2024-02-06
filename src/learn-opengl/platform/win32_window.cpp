#include "pch.h"
#include "window.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

static LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			break;
		}

		default:
			return DefWindowProc(window, message, wParam, lParam);
	}
	return 0;
}

namespace wmcv
{

struct Win32WindowImpl final
{
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	bool m_close;
};

void PumpMessages(Win32WindowImpl& window)
{
	MSG message = {};
	if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE) != FALSE)
	{
		if ( message.message == WM_QUIT )	
		{
			window.m_close = true;
		}

		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

bool IsOpen(const Win32WindowImpl& window)
{
	return window.m_close == false;
}

void* GetNativeHandle(const Win32WindowImpl& window)
{
	return static_cast<void*>(window.m_hWnd);
}
} // namespace ogl_starter

wmcv::Window wmcvCreateWindow(wmcv::WindowCreateParams params)
{
	const char className[] = "Win32WindowImpl";
	HINSTANCE hInstance = GetModuleHandle(NULL);
	wmcv::Win32WindowImpl result = {
		.m_hInstance = nullptr,
		.m_hWnd = nullptr,
		.m_close = true};

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
		nullptr);

	if (window == nullptr)
	{
		MessageBox(nullptr, "Call to CreateWindow failed", "Fatal Error", MB_OK);
		return result;
	}

	ShowWindow(window, SW_SHOW);

	result.m_hInstance = hInstance;
	result.m_hWnd = window;
	result.m_close = false;

	return result;
}
#pragma once

namespace wmcv
{

class IWindowMessageSink
{
public:
	virtual ~IWindowMessageSink() = default;
	virtual bool HandleWindowMessage(uintptr_t window, uint64_t message, uintptr_t data0, uintptr_t data1) = 0;
};

struct WindowCreateParams
{
	int x;
	int y;
	int width;
	int height;
	const char* name;
};

class IWindow
{
public:
	virtual ~IWindow() = default;

	virtual void PumpMessages() = 0;
	virtual bool IsOpen() const = 0;
	virtual void* GetNativeHandle() const = 0;
	virtual void PushSink(IWindowMessageSink*) = 0;

	static std::unique_ptr<IWindow> Create(const WindowCreateParams& params);
};

using WindowPtr = std::unique_ptr<IWindow>;

} // namespace starter_window

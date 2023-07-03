#pragma once

#include <memory>

namespace ogl_starter
{

struct WindowCreateParams
{
	int x;
	int y;
	int width;
	int height;
	const char* name;
};

template <typename T>
concept IsWindow = requires(T t) {
	PumpMessages(t);
	ShouldClose(t);
	GetNativeHandle(t);
};

class Window
{
public:
	template <IsWindow T>
	Window(T&& t)
		: self{std::make_unique<model_t<T>>(std::move(t))}
	{
	}

	~Window() = default;
	Window(Window&&) = default;
	Window& operator=(Window&&) = default;
	Window(const Window&) = delete;
	Window operator=(const Window&) = delete;

	friend void PumpMessages(Window& window) { window.self->PumpMessages_(); }
	friend bool ShouldClose(const Window& window) { return window.self->ShouldClose_(); }
	friend void* GetNativeHandle(const Window& window) { return window.self->GetNativeHandle_(); }

private:
	struct concept_t
	{
		virtual ~concept_t() = default;
		virtual void PumpMessages_() = 0;
		virtual bool ShouldClose_() const = 0;
		virtual void* GetNativeHandle_() const = 0;
	};

	template <typename T>
	struct model_t final : concept_t
	{
		model_t(T&& data) : m_data(std::move(data)) {}

		virtual void PumpMessages_() override { PumpMessages(m_data); }
		virtual bool ShouldClose_() const override { return ShouldClose(m_data); }
		virtual void* GetNativeHandle_() const override { return GetNativeHandle(m_data); }

		T m_data;
	};

	std::unique_ptr<concept_t> self;
};

} // namespace starter_window

ogl_starter::Window oglsCreateWindow(ogl_starter::WindowCreateParams params);

#pragma once

#include <memory>

namespace ogl_starter
{

struct OpenGLCreateParams
{
	void* nativeWindowHandle;
	int versionMajor;
	int versionMinor;
};

template <typename T>
concept IsOpenGL = requires(T t) {
	ClearBuffers(t);
	Present(t);
	DrawScene(t);
};

class OpenGL
{
public:
	template <IsOpenGL T>
	OpenGL(T&& t)
		: self{std::make_unique<model_t<T>>(std::move(t))}
	{
	}

	~OpenGL() = default;
	OpenGL(OpenGL&&) = default;
	OpenGL& operator=(OpenGL&&) = default;
	OpenGL(const OpenGL&) = delete;
	OpenGL operator=(const OpenGL&) = delete;

	friend void ClearBuffers(const OpenGL& opengl) { opengl.self->ClearBuffers_(); }
	friend void Present(const OpenGL& opengl) { opengl.self->Present_(); }
	friend void DrawScene(const OpenGL& opengl) { opengl.self->DrawScene_(); }
	friend void Destroy(const OpenGL& opengl) { opengl.self->Destroy_(); }

private:
	struct concept_t
	{
		virtual ~concept_t() = default;

		virtual void ClearBuffers_() const = 0;
		virtual void Present_() const = 0;
		virtual void DrawScene_() const = 0;
		virtual void Destroy_() const = 0;
	};

	template <typename T>
	struct model_t final : concept_t
	{
		model_t(T&& data) : m_data(std::move(data)) {}

		virtual void ClearBuffers_() const override { ClearBuffers(m_data); }
		virtual void Present_() const override { Present(m_data); }
		virtual void DrawScene_() const override { DrawScene(m_data); }
		virtual void Destroy_() const override { Destroy(m_data); }

		T m_data;
	};

	std::unique_ptr<concept_t> self;
};

} // namespace ogl_starter

ogl_starter::OpenGL oglsCreateOpenGL(ogl_starter::OpenGLCreateParams params);

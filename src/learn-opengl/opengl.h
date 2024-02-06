#pragma once

#include <GL/GL.h>
#include "GL/glext.h"
#include "GL/wglext.h"

namespace wmcv
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
	friend void DrawScene(OpenGL& opengl) { opengl.self->DrawScene_(); }
	friend void Destroy(const OpenGL& opengl) { opengl.self->Destroy_(); }

private:
	struct concept_t
	{
		virtual ~concept_t() = default;

		virtual void ClearBuffers_() const = 0;
		virtual void Present_() const = 0;
		virtual void DrawScene_() = 0;
		virtual void Destroy_() const = 0;
	};

	template <typename T>
	struct model_t final : concept_t
	{
		model_t(T&& data) : m_data(std::move(data)) {}

		virtual void ClearBuffers_() const override { ClearBuffers(m_data); }
		virtual void Present_() const override { Present(m_data); }
		virtual void DrawScene_() override { DrawScene(m_data); }
		virtual void Destroy_() const override { Destroy(m_data); }

		T m_data;
	};

	std::unique_ptr<concept_t> self;
};

} // namespace ogl_starter

extern PFNWGLGETEXTENSIONSSTRINGEXTPROC wglGetExtensionsStringEXT;
extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLUNIFORM1FPROC glUniform1f;
extern PFNGLUNIFORM4FPROC glUniform4f;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;

wmcv::OpenGL wmcvCreateOpenGL(wmcv::OpenGLCreateParams params);

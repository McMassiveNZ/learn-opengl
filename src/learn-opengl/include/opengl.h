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
	SetClearColor(t, float{}, float{}, float{}),
	SetOpacity(t, float{});
	SetModelTransform(t, glm::mat4{});
	SetViewTransform(t, glm::mat4{});
	SetProjectionTransform(t, glm::mat4{});
	SetLightTransform(t, glm::mat4{});
	SetViewPosition(t, glm::vec3{});
};

class OpenGL
{
public:
	OpenGL() = default;

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
	friend void SetClearColor(OpenGL& opengl, float r, float g, float b) { opengl.self->SetClearColor_(r, g, b); }

	friend void SetOpacity(OpenGL& opengl, float opacity) { opengl.self->SetOpacity_(opacity); }
	friend void SetModelTransform(OpenGL& opengl, const glm::mat4& transform) { opengl.self->SetModelTransform_(transform); }
	friend void SetViewTransform(OpenGL& opengl, const glm::mat4& transform) { opengl.self->SetViewTransform_(transform); }
	friend void SetProjectionTransform(OpenGL& opengl, const glm::mat4& transform) { opengl.self->SetProjectionTransform_(transform); }

	friend void SetLightTransform(OpenGL& opengl, const glm::mat4& transform) { opengl.self->SetLightTransform_(transform); }
	friend void SetLightColor(OpenGL& opengl, const glm::vec3& color) { opengl.self->SetLightColor_(color); }
	friend void SetViewPosition(OpenGL& opengl, const glm::vec3& pos) { opengl.self->SetViewPosition_(pos); }

private:
	struct concept_t
	{
		virtual ~concept_t() = default;

		virtual void ClearBuffers_() const = 0;
		virtual void Present_() const = 0;
		virtual void DrawScene_() = 0;
		virtual void Destroy_() const = 0;
		virtual void SetClearColor_(float, float, float) = 0;
		virtual void SetOpacity_(float) = 0;
		virtual void SetModelTransform_(const glm::mat4&) = 0;
		virtual void SetViewTransform_(const glm::mat4&) = 0;
		virtual void SetProjectionTransform_(const glm::mat4&) = 0;

		virtual void SetLightTransform_(const glm::mat4&) = 0;
		virtual void SetLightColor_(const glm::vec3&) = 0;
		virtual void SetViewPosition_(const glm::vec3&) = 0;
	};

	template <typename T>
	struct model_t final : concept_t
	{
		model_t(T&& data) : m_data(std::move(data)) {}

		virtual void ClearBuffers_() const override { ClearBuffers(m_data); }
		virtual void Present_() const override { Present(m_data); }
		virtual void DrawScene_() override { DrawScene(m_data); }
		virtual void Destroy_() const override { Destroy(m_data); }
		virtual void SetClearColor_(float r, float g, float b) { SetClearColor(m_data, r, g, b); }

		virtual void SetOpacity_( float opacity ) override { SetOpacity(m_data, opacity); }
		virtual void SetModelTransform_( const glm::mat4& transform ) override { SetModelTransform(m_data, transform); }
		virtual void SetViewTransform_( const glm::mat4& transform ) override { SetViewTransform(m_data, transform); }
		virtual void SetProjectionTransform_( const glm::mat4& transform ) override { SetProjectionTransform(m_data, transform); }
		virtual void SetLightTransform_( const glm::mat4& transform ) override { SetLightTransform(m_data, transform); }
		virtual void SetLightColor_(const glm::vec3& color) override { SetLightColor(m_data, color); }
		virtual void SetViewPosition_(const glm::vec3& pos) override { SetViewPosition(m_data, pos); }

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
extern PFNGLUNIFORM1FPROC glUniform1f;
extern PFNGLUNIFORM2FPROC glUniform2f;
extern PFNGLUNIFORM3FPROC glUniform3f;
extern PFNGLUNIFORM4FPROC glUniform4f;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLUNIFORM2IPROC glUniform2i;
extern PFNGLUNIFORM3IPROC glUniform3i;
extern PFNGLUNIFORM4IPROC glUniform4i;
extern PFNGLUNIFORM1FVPROC glUniform1fv;
extern PFNGLUNIFORM2FVPROC glUniform2fv;
extern PFNGLUNIFORM3FVPROC glUniform3fv;
extern PFNGLUNIFORM4FVPROC glUniform4fv;
extern PFNGLUNIFORM1IVPROC glUniform1iv;
extern PFNGLUNIFORM2IVPROC glUniform2iv;
extern PFNGLUNIFORM3IVPROC glUniform3iv;
extern PFNGLUNIFORM4IVPROC glUniform4iv;
extern PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
extern PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
extern PFNGLACTIVETEXTUREPROC glActiveTexture;

wmcv::OpenGL wmcvCreateOpenGL(wmcv::OpenGLCreateParams params);

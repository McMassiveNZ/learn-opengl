#include "pch.h"
#include "opengl.h"
#include "shader.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>

namespace fs = std::filesystem;

template <typename T>
static T LoadGLProc(const char* functionName)
{
	FARPROC proc = wglGetProcAddress(functionName);
	return static_cast<T>(static_cast<void*>(proc));
}

#define VALIDATEGLPROC(proc) \
if (!proc) \
{ \
	MessageBox(NULL, "Failed to load " #proc, "Fatal Error", MB_ICONERROR); \
}

PFNWGLGETEXTENSIONSSTRINGEXTPROC wglGetExtensionsStringEXT = nullptr;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback = nullptr;
PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
PFNGLCREATESHADERPROC glCreateShader = nullptr;
PFNGLDELETESHADERPROC glDeleteShader = nullptr;
PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
PFNGLUNIFORM1IPROC glUniform1i = nullptr;
PFNGLUNIFORM1FPROC glUniform1f = nullptr;
PFNGLUNIFORM4FPROC glUniform4f = nullptr;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = nullptr;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC glBufferData = nullptr;

namespace wmcv
{

static void LoadGLFunctions()
{
	glDebugMessageCallback = LoadGLProc<PFNGLDEBUGMESSAGECALLBACKPROC>("glDebugMessageCallback");

	glCreateProgram = LoadGLProc<PFNGLCREATEPROGRAMPROC>("glCreateProgram");
	glDeleteProgram = LoadGLProc<PFNGLDELETEPROGRAMPROC>("glDeleteProgram");
	glAttachShader = LoadGLProc<PFNGLATTACHSHADERPROC>("glAttachShader");
	glLinkProgram = LoadGLProc<PFNGLLINKPROGRAMPROC>("glLinkProgram");
	glUseProgram = LoadGLProc<PFNGLUSEPROGRAMPROC>("glUseProgram");
	glGetProgramiv = LoadGLProc<PFNGLGETPROGRAMIVPROC>("glGetProgramiv");
	glGetProgramInfoLog = LoadGLProc<PFNGLGETPROGRAMINFOLOGPROC>("glGetProgramInfoLog");

	glCreateShader = LoadGLProc<PFNGLCREATESHADERPROC>("glCreateShader");
	glDeleteShader = LoadGLProc<PFNGLDELETESHADERPROC>("glDeleteShader");
	glCompileShader = LoadGLProc<PFNGLCOMPILESHADERPROC>("glCompileShader");
	glShaderSource = LoadGLProc<PFNGLSHADERSOURCEPROC>("glShaderSource");
	glGetShaderiv = LoadGLProc<PFNGLGETSHADERIVPROC>("glGetShaderiv");
	glGetShaderInfoLog = LoadGLProc<PFNGLGETSHADERINFOLOGPROC>("glGetShaderInfoLog");
	glGetUniformLocation = LoadGLProc<PFNGLGETUNIFORMLOCATIONPROC>("glGetUniformLocation");

	glUniform1i = LoadGLProc<PFNGLUNIFORM1IPROC>("glUniform1i");
	glUniform1f = LoadGLProc<PFNGLUNIFORM1FPROC>("glUniform1f");
	glUniform4f = LoadGLProc<PFNGLUNIFORM4FPROC>("glUniform4f");

	glGenVertexArrays = LoadGLProc<PFNGLGENVERTEXARRAYSPROC>("glGenVertexArrays");
	glDeleteVertexArrays = LoadGLProc<PFNGLDELETEVERTEXARRAYSPROC>("glDeleteVertexArrays");
	glBindVertexArray = LoadGLProc<PFNGLBINDVERTEXARRAYPROC>("glBindVertexArray");
	glVertexAttribPointer = LoadGLProc<PFNGLVERTEXATTRIBPOINTERPROC>("glVertexAttribPointer");
	glEnableVertexAttribArray = LoadGLProc<PFNGLENABLEVERTEXATTRIBARRAYPROC>("glEnableVertexAttribArray");

	glGenBuffers = LoadGLProc<PFNGLGENBUFFERSPROC>("glGenBuffers");
	glBindBuffer = LoadGLProc<PFNGLBINDBUFFERPROC>("glBindBuffer");
	glBufferData = LoadGLProc<PFNGLBUFFERDATAPROC>("glBufferData");
}

static GLuint InitTriangleResources()
{
	float vertices[] = {
		0.5f,  0.5f, 0.0f,  1.f, 0.f, 0.f,
	   -0.5f,  0.5f, 0.0f,  0.f, 1.f, 0.f,
		0.0f, -0.5f, 0.0f,  0.f, 0.f, 1.f
	};

	unsigned int indices[] = {
		0, 1, 2,
		//1, 2, 3
	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return VAO;
 }

struct Win32OpenGLImpl final
{
	HDC m_deviceContext;
	HGLRC m_renderingContext;
	HWND m_windowHandle;

	GLuint VAO;
	wmcv::Shader shaderProgram;
};

void ClearBuffers(const Win32OpenGLImpl&)
{
	glClearColor(0.129f, 0.586f, 0.949f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Present(const Win32OpenGLImpl& opengl)
{
	SwapBuffers(opengl.m_deviceContext);
}

void DrawScene(Win32OpenGLImpl& opengl)
{
	opengl.shaderProgram.use();

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

	glUseProgram(0);
}

void Destroy(const Win32OpenGLImpl& opengl)
{
	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(opengl.m_renderingContext);
}
} // namespace ogl_starter

wmcv::OpenGL wmcvCreateOpenGL(wmcv::OpenGLCreateParams params)
{
	auto hWnd = static_cast<HWND>(params.nativeWindowHandle);
	auto hInstance = GetModuleHandle(NULL);

	HWND dummyWindow = [hInstance]() -> HWND
	{
		WNDCLASSEX wcex;
		ZeroMemory(&wcex, sizeof(wcex));
		wcex.cbSize = sizeof(wcex);
		wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wcex.lpfnWndProc = DefWindowProc;
		wcex.hInstance = hInstance;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.lpszClassName = "DummyWNDClass";

		if (RegisterClassEx(&wcex) == 0)
		{
			MessageBox(NULL, "registerClass() failed.", "Fatal Error", MB_ICONERROR);
			return nullptr;
		}

		return CreateWindow(
			wcex.lpszClassName, "Dummy Window",
			WS_OVERLAPPEDWINDOW,
			0, 0,
			1, 1,
			nullptr, nullptr,
			hInstance, nullptr);
	}();

	HDC dummyDC = GetDC(dummyWindow);

	PIXELFORMATDESCRIPTOR dummyPFD = {};
	dummyPFD.nSize = sizeof(dummyPFD);
	dummyPFD.nVersion = 1;
	dummyPFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	dummyPFD.iPixelType = PFD_TYPE_RGBA;
	dummyPFD.cColorBits = 32;
	dummyPFD.cAlphaBits = 8;
	dummyPFD.cDepthBits = 24;
	dummyPFD.iLayerType = PFD_MAIN_PLANE;

	const int dummyPFDID = ChoosePixelFormat(dummyDC, &dummyPFD);
	if (dummyPFDID == 0)
	{
		MessageBox(nullptr, "ChoosePixelFormat() failed.", "Fatal Error", MB_ICONERROR);
	}

	if (SetPixelFormat(dummyDC, dummyPFDID, &dummyPFD) == false)
	{
		MessageBox(nullptr, "SetPixelFormat() failed.", "Fatal Error", MB_ICONERROR);
	}

	HGLRC dummyRC = wglCreateContext(dummyDC);

	if (dummyRC == 0)
	{
		MessageBox(nullptr, "wglCreateContext() failed.", "Fatal Error", MB_ICONERROR);
	}

	if (wglMakeCurrent(dummyDC, dummyRC) == false)
	{
		MessageBox(nullptr, "wglMakeCurrent() failed.", "Fatal Error", MB_ICONERROR);
	}

	wglChoosePixelFormatARB = LoadGLProc<PFNWGLCHOOSEPIXELFORMATARBPROC>("wglChoosePixelFormatARB");
	VALIDATEGLPROC(wglChoosePixelFormatARB);

	wglCreateContextAttribsARB = LoadGLProc<PFNWGLCREATECONTEXTATTRIBSARBPROC>("wglCreateContextAttribsARB");
	VALIDATEGLPROC(wglCreateContextAttribsARB);

	auto deviceContext = GetDC(hWnd);

	const int pixelAttribs[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		WGL_SAMPLES_ARB, 4,
		0};

	int pixelFormatID;
	UINT numFormats;
	const bool status = wglChoosePixelFormatARB(deviceContext, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);

	if (status == false || numFormats == 0)
	{
		MessageBox(nullptr, "wglChoosePixelFormatARB() failed.", "Fatal Error", MB_ICONERROR);
	}

	PIXELFORMATDESCRIPTOR PFD;
	DescribePixelFormat(deviceContext, pixelFormatID, sizeof(PFD), &PFD);
	SetPixelFormat(deviceContext, pixelFormatID, &PFD);

	const int major_min = 4, minor_min = 6;
	const int contextAttribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
		WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		WGL_CONTEXT_LAYER_PLANE_ARB, 0,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
		0};

	auto renderingContext = wglCreateContextAttribsARB(deviceContext, 0, contextAttribs);
	if (renderingContext == nullptr)
	{
		MessageBox(nullptr, "wglCreateContextAttribsARB() failed.", "Fatal Error", MB_ICONERROR);
	}

	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(dummyRC);
	ReleaseDC(dummyWindow, dummyDC);
	DestroyWindow(dummyWindow);

	if (!wglMakeCurrent(deviceContext, renderingContext))
	{
		MessageBox(nullptr, "wglMakeCurrent() failed.", "Fatal Error", MB_ICONERROR);
	}

	wmcv::LoadGLFunctions();

	const auto shader_path_dir = 
		fs::current_path().root_name() /
		fs::current_path().root_directory() / 
		fs::path("Users") /
		fs::path("dev") /
		fs::path("git") / 
		fs::path("learn-opengl") / 
		fs::path("src") / 
		fs::path("learn-opengl");

	const auto vs_path = (shader_path_dir / fs::path("vert.txt")).make_preferred();
	const auto fs_path = (shader_path_dir / fs::path("frag.txt")).make_preferred();

	auto result = wmcv::Win32OpenGLImpl
	{
		.m_deviceContext = deviceContext,
		.m_renderingContext = renderingContext,
		.m_windowHandle = hWnd,
		.VAO = wmcv::InitTriangleResources(),
		.shaderProgram = wmcv::Shader(vs_path, fs_path)
	};

	return result;
}
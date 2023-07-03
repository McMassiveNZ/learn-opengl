#include "opengl.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>
#include <GL/GL.h>
#include "GL/glext.h"
#include "GL/wglext.h"

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

static PFNWGLGETEXTENSIONSSTRINGEXTPROC wglGetExtensionsStringEXT;
static PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

static PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback;

static PFNGLCREATEPROGRAMPROC glCreateProgram;
static PFNGLDELETEPROGRAMPROC glDeleteProgram;
static PFNGLATTACHSHADERPROC glAttachShader;
static PFNGLLINKPROGRAMPROC glLinkProgram;
static PFNGLUSEPROGRAMPROC glUseProgram;
static PFNGLGETPROGRAMIVPROC glGetProgramiv;
static PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
static PFNGLCREATESHADERPROC glCreateShader;
static PFNGLDELETESHADERPROC glDeleteShader;
static PFNGLCOMPILESHADERPROC glCompileShader;
static PFNGLSHADERSOURCEPROC glShaderSource;
static PFNGLGETSHADERIVPROC glGetShaderiv;
static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;

static PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
static PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
static PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
static PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
static PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;

static PFNGLGENBUFFERSPROC glGenBuffers;
static PFNGLBINDBUFFERPROC glBindBuffer;
static PFNGLBUFFERDATAPROC glBufferData;

namespace ogl_starter
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

	glGenVertexArrays = LoadGLProc<PFNGLGENVERTEXARRAYSPROC>("glGenVertexArrays");
	glDeleteVertexArrays = LoadGLProc<PFNGLDELETEVERTEXARRAYSPROC>("glDeleteVertexArrays");
	glBindVertexArray = LoadGLProc<PFNGLBINDVERTEXARRAYPROC>("glBindVertexArray");
	glVertexAttribPointer = LoadGLProc<PFNGLVERTEXATTRIBPOINTERPROC>("glVertexAttribPointer");
	glEnableVertexAttribArray = LoadGLProc<PFNGLENABLEVERTEXATTRIBARRAYPROC>("glEnableVertexAttribArray");

	glGenBuffers = LoadGLProc<PFNGLGENBUFFERSPROC>("glGenBuffers");
	glBindBuffer = LoadGLProc<PFNGLBINDBUFFERPROC>("glBindBuffer");
	glBufferData = LoadGLProc<PFNGLBUFFERDATAPROC>("glBufferData");
}

static std::pair<GLuint, GLuint> InitTriangleResources()
{
	float vertices[] = {
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	auto VS = glCreateShader(GL_VERTEX_SHADER);
	auto FS = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vs_source =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	glShaderSource(VS, 1, &vs_source, nullptr);
	glCompileShader(VS);

	int success;
	char infoLog[512];
	glGetShaderiv(VS, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(VS, 512, nullptr, infoLog);
		printf("Vertex Shader Compilation Failed\n %s\n", infoLog);
	}

	auto fs_source =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";


	glShaderSource(FS, 1, &fs_source, nullptr);
	glCompileShader(FS);
	glGetShaderiv(FS, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(VS, 512, nullptr, infoLog);
		printf("Fragment Shader Compilation Failed\n %s\n", infoLog);
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, VS);
	glAttachShader(shaderProgram, FS);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		printf("Shader Program Link Failed\n %s\n", infoLog);
	}

	glUseProgram(shaderProgram);
	glDeleteShader(VS);
	glDeleteShader(FS);

	return {VAO, shaderProgram};
 }

struct Win32OpenGLImpl final
{
	HDC m_deviceContext;
	HGLRC m_renderingContext;
	HWND m_windowHandle;

	GLuint VAO;
	GLuint shaderProgram;
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

void DrawScene(const Win32OpenGLImpl&)
{
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Destroy(const Win32OpenGLImpl& opengl)
{
	glDeleteShader(opengl.shaderProgram);
	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(opengl.m_renderingContext);
}
} // namespace ogl_starter

ogl_starter::OpenGL oglsCreateOpenGL(ogl_starter::OpenGLCreateParams params)
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

	ogl_starter::LoadGLFunctions();
	auto[VAO, shaderProgram] = ogl_starter::InitTriangleResources();

	auto result = ogl_starter::Win32OpenGLImpl(
		deviceContext,
		renderingContext,
		hWnd,
		VAO,
		shaderProgram
	);

	return std::move(result);
}
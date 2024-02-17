#include "pch.h"
#include "opengl.h"
#include "shader.h"
#include "texture.h"

#include "wmcv_log/wmcv_log.h"

namespace fs = std::filesystem;

template <typename T>
static T LoadGLProc(const char* functionName)
{
	PROC proc = wglGetProcAddress(functionName);
	return reinterpret_cast<T>(proc);
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
PFNGLUNIFORM1FPROC glUniform1f = nullptr;
PFNGLUNIFORM2FPROC glUniform2f = nullptr;
PFNGLUNIFORM3FPROC glUniform3f = nullptr;
PFNGLUNIFORM4FPROC glUniform4f = nullptr;
PFNGLUNIFORM1IPROC glUniform1i = nullptr;
PFNGLUNIFORM2IPROC glUniform2i = nullptr;
PFNGLUNIFORM3IPROC glUniform3i = nullptr;
PFNGLUNIFORM4IPROC glUniform4i = nullptr;
PFNGLUNIFORM1FVPROC glUniform1fv = nullptr;
PFNGLUNIFORM2FVPROC glUniform2fv = nullptr;
PFNGLUNIFORM3FVPROC glUniform3fv = nullptr;
PFNGLUNIFORM4FVPROC glUniform4fv = nullptr;
PFNGLUNIFORM1IVPROC glUniform1iv = nullptr;
PFNGLUNIFORM2IVPROC glUniform2iv = nullptr;
PFNGLUNIFORM3IVPROC glUniform3iv = nullptr;
PFNGLUNIFORM4IVPROC glUniform4iv = nullptr;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv = nullptr;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = nullptr;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = nullptr;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC glBufferData = nullptr;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = nullptr;
PFNGLACTIVETEXTUREPROC glActiveTexture = nullptr;

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

	glUniform1f = LoadGLProc<PFNGLUNIFORM1FPROC>("glUniform1f");
	glUniform2f = LoadGLProc<PFNGLUNIFORM2FPROC>("glUniform2f");
	glUniform3f = LoadGLProc<PFNGLUNIFORM3FPROC>("glUniform3f"); 
	glUniform4f = LoadGLProc<PFNGLUNIFORM4FPROC>("glUniform4f");

	glUniform1i = LoadGLProc<PFNGLUNIFORM1IPROC>("glUniform1i"); 
	glUniform2i = LoadGLProc<PFNGLUNIFORM2IPROC>("glUniform2i"); 
	glUniform3i = LoadGLProc<PFNGLUNIFORM3IPROC>("glUniform3i"); 
	glUniform4i = LoadGLProc<PFNGLUNIFORM4IPROC>("glUniform4i"); 

	glUniform1fv = LoadGLProc<PFNGLUNIFORM1FVPROC>("glUniform1fv");
	glUniform2fv = LoadGLProc<PFNGLUNIFORM2FVPROC>("glUniform2fv");
	glUniform3fv = LoadGLProc<PFNGLUNIFORM3FVPROC>("glUniform3fv");
	glUniform4fv = LoadGLProc<PFNGLUNIFORM4FVPROC>("glUniform4fv");

	glUniform1iv = LoadGLProc<PFNGLUNIFORM1IVPROC>("glUniform1iv");
	glUniform2iv = LoadGLProc<PFNGLUNIFORM2IVPROC>("glUniform2iv");
	glUniform3iv = LoadGLProc<PFNGLUNIFORM3IVPROC>("glUniform3iv");
	glUniform4iv = LoadGLProc<PFNGLUNIFORM4IVPROC>("glUniform4iv");

	glUniformMatrix2fv = LoadGLProc<PFNGLUNIFORMMATRIX2FVPROC>("glUniformMatrix2fv");
	glUniformMatrix3fv = LoadGLProc<PFNGLUNIFORMMATRIX3FVPROC>("glUniformMatrix3fv");
	glUniformMatrix4fv = LoadGLProc<PFNGLUNIFORMMATRIX4FVPROC>("glUniformMatrix4fv");

	glGenVertexArrays = LoadGLProc<PFNGLGENVERTEXARRAYSPROC>("glGenVertexArrays");
	glDeleteVertexArrays = LoadGLProc<PFNGLDELETEVERTEXARRAYSPROC>("glDeleteVertexArrays");
	glBindVertexArray = LoadGLProc<PFNGLBINDVERTEXARRAYPROC>("glBindVertexArray");
	glVertexAttribPointer = LoadGLProc<PFNGLVERTEXATTRIBPOINTERPROC>("glVertexAttribPointer");
	glEnableVertexAttribArray = LoadGLProc<PFNGLENABLEVERTEXATTRIBARRAYPROC>("glEnableVertexAttribArray");

	glGenBuffers = LoadGLProc<PFNGLGENBUFFERSPROC>("glGenBuffers");
	glBindBuffer = LoadGLProc<PFNGLBINDBUFFERPROC>("glBindBuffer");
	glBufferData = LoadGLProc<PFNGLBUFFERDATAPROC>("glBufferData");

	glGenerateMipmap = LoadGLProc<PFNGLGENERATEMIPMAPPROC>("glGenerateMipmap");
	glActiveTexture = LoadGLProc<PFNGLACTIVETEXTUREPROC>("glActiveTexture");
}

static std::pair<GLuint, GLuint> InitGPUResources()
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,

		0.5f, 0.5f,   0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f,  -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f,   0.5f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f};

	// first, configure the cube's VAO (and VBO)
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	return std::make_pair(cubeVAO, lightCubeVAO);
}

struct Win32OpenGLImpl final
{
	HDC m_deviceContext;
	HGLRC m_renderingContext;
	HWND m_windowHandle;

	GLuint lightVAO;
	GLuint cubeVAO;
	wmcv::Shader lightingShader;
	wmcv::Shader lightCubeShader;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	glm::mat4 lightTransform;
	glm::vec3 lightColor;
	glm::vec3 viewPosition;

	float mixValue;

	//clear color
	float r, g, b;
};

void ClearBuffers(const Win32OpenGLImpl& opengl)
{
	glClearColor(opengl.r, opengl.g, opengl.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Present(const Win32OpenGLImpl& opengl)
{
	SwapBuffers(opengl.m_deviceContext);
}

void DrawScene(Win32OpenGLImpl& opengl)
{
	// be sure to activate shader when setting uniforms/drawing objects
	opengl.lightingShader.on();
	opengl.lightingShader.setVec3("viewPos", glm::vec3{opengl.viewPosition});

	opengl.lightingShader.setVec3("light.position", glm::vec3{opengl.lightTransform[3]});
	opengl.lightingShader.setVec3("light.ambient", opengl.lightColor * glm::vec3{.2f});
	opengl.lightingShader.setVec3("light.diffuse", opengl.lightColor * glm::vec3{.5f});
	opengl.lightingShader.setVec3("light.specular", glm::vec3{1.f});

	opengl.lightingShader.setVec3("material.ambient", glm::vec3{1.f, 0.5f, 0.31f});
	opengl.lightingShader.setVec3("material.diffuse", glm::vec3{1.f, 0.5f, 0.31f});
	opengl.lightingShader.setVec3("material.specular", glm::vec3{1.f, 0.5f, 0.31f});
	opengl.lightingShader.setFloat("material.shininess", 32.f);

	// view/projection transformations
	opengl.lightingShader.setMat4("projection", opengl.projection);
	opengl.lightingShader.setMat4("view", opengl.view);
	opengl.lightingShader.setMat4("model", opengl.model);

	// render the cube
	glBindVertexArray(opengl.cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// also draw the lamp object
	opengl.lightCubeShader.on();
	opengl.lightCubeShader.setMat4("projection", opengl.projection);
	opengl.lightCubeShader.setMat4("view", opengl.view);
	opengl.lightCubeShader.setMat4("model", opengl.lightTransform);

	glBindVertexArray(opengl.lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Destroy(const Win32OpenGLImpl& opengl)
{
	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(opengl.m_renderingContext);
}

void SetClearColor( Win32OpenGLImpl& opengl, float r, float g, float b)
{
	opengl.r = r;
	opengl.g = g;
	opengl.b = b;
}

void SetOpacity([[maybe_unused]] Win32OpenGLImpl& opengl, float opacity)
{
	const float shifted = (opacity * 0.5f) + 0.5f;
	opengl.mixValue = shifted;
}

void SetModelTransform(Win32OpenGLImpl& opengl, const glm::mat4& transform)
{
	opengl.model = transform;
}

void SetViewTransform(Win32OpenGLImpl& opengl, const glm::mat4& transform)
{
	opengl.view = transform;
}

void SetProjectionTransform(Win32OpenGLImpl& opengl, const glm::mat4& transform)
{
	opengl.projection = transform;
}

void SetLightTransform(Win32OpenGLImpl& opengl, const glm::mat4& transform)
{
	opengl.lightTransform = transform;
}

void SetLightColor(Win32OpenGLImpl& opengl, const glm::vec3& color)
{
	opengl.lightColor = color;
}

void SetViewPosition(Win32OpenGLImpl& opengl, const glm::vec3& position)
{
	opengl.viewPosition = position;
}
} // namespace wmcv

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

	glEnable(GL_DEPTH_TEST);

	const auto data_path_dir =
		fs::current_path().root_name() /
		fs::current_path().root_directory() /
		fs::path("Users") /
		fs::path("dev") /
		fs::path("git") /
		fs::path("learn-opengl") /
		fs::path("data");

	const auto shader_path_dir =
		data_path_dir /
		fs::path("shaders");

	const auto cube_vs_path = (shader_path_dir / fs::path("basic_lighting.vs")).make_preferred();
	const auto cube_fs_path = (shader_path_dir / fs::path("basic_lighting.fs")).make_preferred();
	const auto light_vs_path = (shader_path_dir / fs::path("light_cube.vs")).make_preferred();
	const auto light_fs_path = (shader_path_dir / fs::path("light_cube.fs")).make_preferred();

	auto [cubeVAO, lightVAO] = wmcv::InitGPUResources();

	auto result = wmcv::Win32OpenGLImpl{
		.m_deviceContext = deviceContext,
		.m_renderingContext = renderingContext,
		.m_windowHandle = hWnd,
		.lightVAO = lightVAO,
		.cubeVAO = cubeVAO,
		.lightingShader = wmcv::Shader(cube_vs_path, cube_fs_path),
		.lightCubeShader = wmcv::Shader(light_vs_path, light_fs_path),
	};

	return result;
}
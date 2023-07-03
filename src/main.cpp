#include "window.h"
#include "opengl.h"

constexpr int CW_USEDEFAULT = 0x80000000;

auto main() -> int
{
	auto window = oglsCreateWindow({
		.x = CW_USEDEFAULT,
		.y = CW_USEDEFAULT,
		.width = CW_USEDEFAULT,
		.height = CW_USEDEFAULT,
		.name = "Starter OpenGL"
	});

	auto renderer = oglsCreateOpenGL({
		.nativeWindowHandle = GetNativeHandle(window), 
		.versionMajor = 4, 
		.versionMinor = 6
	});
		
	while (!ShouldClose(window))
	{
		PumpMessages(window);

		ClearBuffers(renderer);
		DrawScene(renderer);
		Present(renderer);
	}

	Destroy(renderer);
	return 0;
}

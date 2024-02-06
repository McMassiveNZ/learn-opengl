#include "pch.h"

#include "window.h"
#include "opengl.h"

#include "wmcv_log/wmcv_log.h"
#include "wmcv_log/sinks/wmcv_sink_outputdbgstring.h"

auto main() -> int
{
	using namespace std::chrono_literals;

	wmcv::CreateDefaultLogSystem();
	wmcv::GetLogSystem().PushSink(wmcv::LogSinkOutputDebugString{});
	wmcv::LogMessage("{}, this is a test, {} {} {} {}", "message", 1, 2.f, 3.0, 4);

	auto window = wmcvCreateWindow({
		.x = CW_USEDEFAULT,
		.y = CW_USEDEFAULT,
		.width = CW_USEDEFAULT,
		.height = CW_USEDEFAULT,
		.name = "Learn OpenGL"
	});

	auto renderer = wmcvCreateOpenGL({
		.nativeWindowHandle = GetNativeHandle(window), 
		.versionMajor = 4, 
		.versionMinor = 6
	});
		
	auto last = std::chrono::high_resolution_clock::now();
	while (IsOpen(window))
	{
		PumpMessages(window);

		ClearBuffers(renderer);
		DrawScene(renderer);
		Present(renderer);

		const auto current = std::chrono::high_resolution_clock::now();
		const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current - last);
		last = current;
		if (elapsed < 16ms)
		{
			const auto difference = 16ms - elapsed;
			std::this_thread::sleep_for(difference);
		}
	}

	Destroy(renderer);
	return 0;
}

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

	auto window = wmcvCreateWindow({
		.x = CW_USEDEFAULT,
		.y = CW_USEDEFAULT,
		.width = 800,
		.height = 600,
		.name = "Learn OpenGL"
	});

	auto renderer = wmcvCreateOpenGL({
		.nativeWindowHandle = GetNativeHandle(window), 
		.versionMajor = 4, 
		.versionMinor = 6
	});

	const auto perspective = glm::perspective(glm::radians(45.f), 800.f / 600.f, 0.01f, 1000.f);
	SetProjectionTransform(renderer, perspective);

	
	std::array<glm::vec3, 10 > cubePositions = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	glm::mat4 model{1.f};
	glm::mat4 view{1.f};
	view = glm::translate(view, glm::vec3{0.f, 0.f, -3.f});

	using as_float = std::chrono::duration<float>;
	const auto start = std::chrono::high_resolution_clock::now();
	auto last = std::chrono::high_resolution_clock::now();
	float timer = 0.f;
	while (IsOpen(window))
	{
		PumpMessages(window);

		ClearBuffers(renderer);

		SetOpacity(renderer, 0.2f);
		SetViewTransform(renderer, view);

		uint32_t i = 0;
		for (const glm::vec3& pos : cubePositions)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pos);
			float angle = 20.0f * i++;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			SetModelTransform(renderer, model);

			DrawScene(renderer);
		}
		Present(renderer);

		const auto current = std::chrono::high_resolution_clock::now();
		const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current - last);
		last = current;
		timer = as_float(current - start).count();
		if (elapsed < 16ms)
		{
			const auto difference = 16ms - elapsed;
			std::this_thread::sleep_for(difference);
		}

		model = glm::identity<glm::mat4>();
		model = glm::rotate(model, timer * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	}

	Destroy(renderer);
	return 0;
}

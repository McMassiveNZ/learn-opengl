#include "pch.h"
#include "application.h"

#include "opengl.h"
#include "window.h"
#include "input.h"
#include "clock.h"

#include "camera.h"

#include "wmcv_log\wmcv_log.h"

namespace wmcv
{
	static std::array<glm::vec3, 10 > cubePositions = {
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
	
	static std::array<glm::mat4, 10 > cubeMatrices = {
		glm::identity<glm::mat4>(),
		glm::identity<glm::mat4>(),
		glm::identity<glm::mat4>(),
		glm::identity<glm::mat4>(),
		glm::identity<glm::mat4>(),
		glm::identity<glm::mat4>(),
		glm::identity<glm::mat4>(),
		glm::identity<glm::mat4>(),
		glm::identity<glm::mat4>(),
		glm::identity<glm::mat4>()
	};

	class Win32ApplicationImpl final : public IApplication
	{
	public:
		int run();

	private:
		bool startup();
		void shutdown();
		void update();
		void draw();
		bool isRunning() const;

		WindowPtr m_window;
		OpenGL m_renderer;
		InputPtr  m_input;
		Clock m_clock;

		Camera camera;
		bool forward = false;
		bool backward = false;
		bool left = false;
		bool right = false;
		bool up = false;
		bool down = false;
	};

	int Win32ApplicationImpl::run()
	{
		int exitStatus = 0;

		if (startup())
		{
			while (isRunning())
			{
				update();
				draw();
			}
		}
		else
		{
			exitStatus = 1;
		}

		shutdown();

		return exitStatus;
	}

	bool Win32ApplicationImpl::startup()
	{
		m_window = IWindow::Create({
			.x = CW_USEDEFAULT,
			.y = CW_USEDEFAULT,
			.width = 800,
			.height = 600,
			.name = "Learn OpenGL"
		});

		m_renderer = wmcvCreateOpenGL({
			.nativeWindowHandle = m_window->GetNativeHandle(),
			.versionMajor = 4,
			.versionMinor = 6
		});

		m_input = wmcv::Input::Create();

		m_window->PushSink(m_input.get());

		const auto perspective = glm::perspective(glm::radians(45.f), 800.f / 600.f, 0.01f, 1000.f);
		SetProjectionTransform(m_renderer, perspective);

		return true;
	}

	void Win32ApplicationImpl::shutdown()
	{
		Destroy(m_renderer);
	}

	void Win32ApplicationImpl::update()
	{
		m_window->PumpMessages();
		m_clock.nextFrame();

		const float delta = m_clock.dt();

		auto events = m_input->FlushEvents();
		for (const auto& evt : events)
		{
			if (evt.key == InputKey::ESCAPE && evt.type == InputEventType::KeyUp)
			{
				if (m_input->IsCursorHidden())
				{
					m_input->ShowCursor();
				}
				else
				{
					m_input->HideCursor();
				}
			}

			if (m_input->IsCursorHidden() == false)
				continue;

			if (evt.type == InputEventType::MouseMove)
				camera.applyCameraLook(static_cast<float>(evt.rel_x), static_cast<float>(evt.rel_y));

			if (evt.type == InputEventType::KeyDown)
			{
				if (evt.key == InputKey::W)
					forward = true;
				if (evt.key == InputKey::A)
					left = true;
				if (evt.key == InputKey::S)
					backward = true;
				if (evt.key == InputKey::D)
					right = true;
				if (evt.key == InputKey::Q)
					up = true;
				if (evt.key == InputKey::E)
					down = true;
			}
			else if (evt.type == InputEventType::KeyUp)
			{
				if (evt.key == InputKey::W)
					forward = false;
				if (evt.key == InputKey::A)
					left = false;
				if (evt.key == InputKey::S)
					backward = false;
				if (evt.key == InputKey::D)
					right = false;
				if (evt.key == InputKey::Q)
					up = false;
				if (evt.key == InputKey::E)
					down = false;
			}
			else if (evt.type == InputEventType::MouseWheel)
			{
				camera.applyCameraZoom(static_cast<float>(evt.wheel * 10));
			}
		}

		if (forward)
			camera.applyCameraMovement(Camera_Movement::FORWARD, delta);
		if (backward) 
			camera.applyCameraMovement(Camera_Movement::BACKWARD, delta);
		if (left)
			camera.applyCameraMovement(Camera_Movement::LEFT, delta);
		if (right) 
			camera.applyCameraMovement(Camera_Movement::RIGHT, delta);
		if (up)
			camera.applyCameraMovement(Camera_Movement::UP, delta);
		if (down)
			camera.applyCameraMovement(Camera_Movement::DOWN, delta);

		for ( uint32_t i = 0; i < cubeMatrices.size(); ++i)
		{
			auto& model = cubeMatrices[i];
			const auto& pos = cubePositions[i];

			model = glm::mat4(1.0f);
			model = glm::translate(model, pos);
			float angle = 20.0f * i;
			model = glm::rotate(model, m_clock.elapsed() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			SetModelTransform(m_renderer, model);
		}
	}

	void Win32ApplicationImpl::draw()
	{
		ClearBuffers(m_renderer);

		SetOpacity(m_renderer, 0.2f);
		SetViewTransform(m_renderer, camera.buildViewMatrix());

		for (const glm::mat4& model : cubeMatrices)
		{
			SetModelTransform(m_renderer, model);
			DrawScene(m_renderer);
		}

		Present(m_renderer);
	}

	bool Win32ApplicationImpl::isRunning() const
	{
		return m_window->IsOpen();
	}

	std::unique_ptr<wmcv::IApplication> IApplication::Create()
	{
		return std::make_unique<wmcv::Win32ApplicationImpl>();
	}
}

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
	struct Light
	{
		glm::mat4 transform;
		glm::vec3 color;
		float speed;
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

		glm::mat4 model;

		Camera camera;
		bool forward = false;
		bool backward = false;
		bool left = false;
		bool right = false;
		bool up = false;
		bool down = false;

		Light light;
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

		light.color = glm::vec3{1.f};
		light.speed = 50.f;

		const auto perspective = glm::perspective(camera.zoom(), 800.f / 600.f, 0.01f, 1000.f);
		SetProjectionTransform(m_renderer, perspective);
		SetClearColor(m_renderer, 0.1f, 0.1f, 0.1f);
		SetLightColor(m_renderer, glm::vec3{1.f});

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

		model = glm::identity<glm::mat4>();

		light.transform = glm::identity<glm::mat4>();
		light.transform = glm::rotate(light.transform, glm::radians(m_clock.elapsed() * light.speed), glm::vec3{0.f, 1.f, 0.f});
		light.transform = glm::translate(light.transform, glm::vec3{1.2f, 1.f, 2.f});
		light.transform = glm::scale(light.transform, glm::vec3{0.2f});
		light.speed = 50.f;
		//light.color = glm::vec3{
		//	sinf(m_clock.elapsed() * 2.0f),
		//	sinf(m_clock.elapsed() * 0.7f),
		//	sinf(m_clock.elapsed() * 1.3f),
		//};
	}

	void Win32ApplicationImpl::draw()
	{
		ClearBuffers(m_renderer);

		SetOpacity(m_renderer, 0.2f);
		SetViewTransform(m_renderer, camera.buildViewMatrix());
		SetModelTransform(m_renderer, model);
		SetLightTransform(m_renderer, light.transform);
		SetLightColor(m_renderer, light.color);
		SetViewPosition(m_renderer, camera.position());
		DrawScene(m_renderer);

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

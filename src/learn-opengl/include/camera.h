#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

namespace wmcv
{

enum class Camera_Movement : uint8_t
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 2.5f;
constexpr float SENSITIVITY = 0.5f;
constexpr float ZOOM = 45.0f;

class Camera
{
public:
	Camera();

	inline glm::mat4 buildViewMatrix() const
	{
		const auto eye = m_position;
		const auto at = eye + m_front;
		const auto up = m_up;
		return glm::lookAt(eye, at, up);
	}

	inline float zoom() const { return m_zoom; }
	inline glm::vec3 position() const { return m_position; }
	inline glm::vec3 front() const { return m_front; } 
	inline glm::vec3 right() const { return m_right; } 
	inline glm::vec3 up() const { return m_up; } 

	void applyCameraMovement(Camera_Movement direction, float dt);
	void applyCameraLook(float yaw, float pitch);
	void applyCameraZoom(float zoom);

private:
	void updateCameraVectors();

	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;

	float m_yaw;
	float m_pitch;
	float m_speed;
	float m_sensitivity;
	float m_zoom;
};
} // namespace wmcv
#endif


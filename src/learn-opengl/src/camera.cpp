#include "pch.h"
#include "camera.h"

namespace wmcv
{

Camera::Camera()
	: m_position(0.f, 0.f, -3.f)
	, m_front(0.f, 0.f, -1.f)
	, m_up{}
	, m_right{}
	, m_worldUp(0.f, 1.f, 0.f)
	, m_yaw(YAW)
	, m_pitch(PITCH)
	, m_speed(SPEED)
	, m_sensitivity(SENSITIVITY)
	, m_zoom(ZOOM)
{
	updateCameraVectors();
}

void Camera::applyCameraMovement(Camera_Movement direction, float dt)
{
	const float velocity = m_speed * dt;

	if (direction == Camera_Movement::FORWARD)
		m_position += m_front * velocity;

	if (direction == Camera_Movement::BACKWARD)
		m_position -= m_front * velocity;

	if (direction == Camera_Movement::LEFT)
		m_position -= m_right * velocity;

	if (direction == Camera_Movement::RIGHT)
		m_position += m_right * velocity;

	if (direction == Camera_Movement::DOWN)
		m_position -= m_up * velocity;

	if (direction == Camera_Movement::UP)
		m_position += m_up * velocity;
}

void Camera::applyCameraLook(float yaw, float pitch)
{
	m_yaw += yaw*m_sensitivity;
	m_pitch -= pitch*m_sensitivity;
	m_pitch = std::clamp(m_pitch, -89.f, 89.f);

	updateCameraVectors();
}

void Camera::applyCameraZoom(float zoom)
{
	m_zoom -= zoom;
	m_zoom = std::clamp(m_zoom, 1.f, 45.f);
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);

	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}
} // namespace wmcv

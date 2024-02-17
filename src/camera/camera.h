#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util/window.h"

class Camera 
{
public:
	Camera(glm::vec3 position, float yaw, float pitch, float movementSpeed, float mouseSensitivity);

	void update(std::shared_ptr<Window> window, float deltaTime);

	void setAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; }

	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;

private:
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;

	float m_yaw;
	float m_pitch;

	float m_movementSpeed;
	float m_mouseSensitivity;

	float m_lastX;
	float m_lastY;
	bool m_firstMouse;

	float m_fov;
	float m_aspectRatio;
	float m_nearPlane;
	float m_farPlane;

	bool m_isMouseCaptured;

	void m_updateCameraVectors();
	void m_processKeyboard(float deltaTime, bool forward, bool backward, bool left, bool right, bool up, bool down);
	void m_processMouseMovement(std::shared_ptr<Window> window);
};

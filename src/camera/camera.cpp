#include "camera.h"

#include <GLFW/glfw3.h>
#include <iostream>

Camera::Camera(glm::vec3 position, float yaw, float pitch, float movementSpeed, float mouseSensitivity)
	: m_position(position), m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_yaw(yaw), m_pitch(pitch), m_movementSpeed(movementSpeed), m_mouseSensitivity(mouseSensitivity), 
	m_fov(60.0f), m_nearPlane(0.01f), m_farPlane(1000.0f), m_firstMouse(true), m_lastX(0.0f), m_lastY(0.0f), m_isMouseCaptured(false)
{
	m_updateCameraVectors();
}

void Camera::update(std::shared_ptr<Window> window, float deltaTime)
{
	if (glfwGetKey(window->get(), GLFW_KEY_W) == GLFW_PRESS)
		m_processKeyboard(deltaTime, true, false, false, false, false, false);
	if (glfwGetKey(window->get(), GLFW_KEY_S) == GLFW_PRESS)
		m_processKeyboard(deltaTime, false, true, false, false, false, false);
	if (glfwGetKey(window->get(), GLFW_KEY_A) == GLFW_PRESS)
		m_processKeyboard(deltaTime, false, false, true, false, false, false);
	if (glfwGetKey(window->get(), GLFW_KEY_D) == GLFW_PRESS)
		m_processKeyboard(deltaTime, false, false, false, true, false, false);
	if (glfwGetKey(window->get(), GLFW_KEY_E) == GLFW_PRESS)
		m_processKeyboard(deltaTime, false, false, false, false, true, false);
	if (glfwGetKey(window->get(), GLFW_KEY_Q) == GLFW_PRESS)
		m_processKeyboard(deltaTime, false, false, false, false, false, true);
	// capture mouse
	if (glfwGetMouseButton(window->get(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		glfwSetInputMode(window->get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_isMouseCaptured = true;
	}
	else
	{
		glfwSetInputMode(window->get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_isMouseCaptured = false;
	}


	m_processMouseMovement(window);
}

void Camera::m_updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	this->m_front = glm::normalize(front);
	m_right = glm::normalize(glm::cross(this->m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, this->m_front));
}

void Camera::m_processKeyboard(float deltaTime, bool forward, bool backward, bool left, bool right, bool up, bool down)
{
	float velocity = m_movementSpeed * deltaTime;
	if (forward)
		m_position += m_front * velocity;
	if (backward)
		m_position -= m_front * velocity;
	if (left)
		m_position -= m_right * velocity;
	if (right)
		m_position += m_right * velocity;
	if (up)
		m_position += m_worldUp * velocity;
	if (down)
		m_position -= m_worldUp * velocity;
	m_updateCameraVectors();
}

void Camera::m_processMouseMovement(std::shared_ptr<Window> window)
{
	double xpos, ypos;
	glfwGetCursorPos(window->get(), &xpos, &ypos);

	if (m_isMouseCaptured)
	{
		if (m_firstMouse)
		{
			m_lastX = xpos;
			m_lastY = ypos;
			m_firstMouse = false;
		}

		float xoffset = xpos - m_lastX;
		float yoffset = m_lastY - ypos;
		m_lastX = xpos;
		m_lastY = ypos;

		xoffset *= m_mouseSensitivity;
		yoffset *= m_mouseSensitivity;

		m_yaw += xoffset;
		m_pitch += yoffset;

		m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);

		m_updateCameraVectors();
	}
	else
	{
		m_firstMouse = true;
	}
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 Camera::getProjectionMatrix(std::shared_ptr<Window> window) const
{

	return glm::perspective(glm::radians(m_fov), 16.0f / 9.0f, m_nearPlane, m_farPlane);
}

#pragma once

#include <memory>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"


class Window
{
public:
	Window(int height, int width, const char* name);

	bool shouldClose() { return glfwWindowShouldClose(m_window); }
	GLFWwindow* get() { return m_window; }
	glm::ivec2 getSize() { return m_size; };
private:

	GLFWwindow* m_window;
	glm::ivec2 m_size;
};

#pragma once

#include <memory>
#include <GLFW/glfw3.h>

namespace core
{
	class Window
	{
	public:
		Window(uint32_t height, uint32_t width, const char* name);

		bool shouldClose() { return glfwWindowShouldClose(m_window); }
		GLFWwindow* get() { return m_window; }

	private:
		GLFWwindow* m_window;
	};
}
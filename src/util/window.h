#pragma once

#include <memory>
#include <GLFW/glfw3.h>

namespace util
{
	class Window
	{
	public:
		Window(int height, int width, const char* name);

		bool shouldClose() { return glfwWindowShouldClose(m_window); }
		GLFWwindow* get() { return m_window; }

	private:
		GLFWwindow* m_window;
	};
}
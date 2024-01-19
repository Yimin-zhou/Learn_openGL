#include "window.h"

namespace util
{
	Window::Window(int height, int width, const char* name)
	{
		glfwInit();
		m_window = glfwCreateWindow(height, width, name, NULL, NULL);
		if (!m_window)
		{
			glfwTerminate();
		}
		glfwMakeContextCurrent(m_window);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	}
}
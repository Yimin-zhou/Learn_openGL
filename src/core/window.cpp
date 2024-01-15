#include "window.h"

namespace core
{
	Window::Window(uint32_t height, uint32_t width, const char* name)
	{
		glfwInit();
		m_window = glfwCreateWindow(height, width, name, NULL, NULL);
		if (!m_window)
		{
			glfwTerminate();
		}
		glfwMakeContextCurrent(m_window);
	}
}
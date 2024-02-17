#include "window.h"

Window::Window(int height, int width, const char* name) :
	m_size({ height, width })
{
	glfwInit();
	m_window = glfwCreateWindow(height, width, name, NULL, NULL);
	if (!m_window)
	{
		glfwTerminate();
	}

	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

	// Center the window
	int xPos = (mode->width - height) / 2;
	int yPos = (mode->height - width) / 2;
	glfwSetWindowPos(m_window, xPos, yPos);

	glfwMakeContextCurrent(m_window);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

}



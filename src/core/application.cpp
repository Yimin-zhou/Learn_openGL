#include <iostream>

#include "application.h"

void framebuffer_size_callback(GLFWwindow* window, int  width, int  height)
{
	const float aspectRatio = 16.0f / 9.0f;
	int w = width;
	int h = static_cast<int>(width / aspectRatio);
	if (h > height)
	{
		h = height;
		w = static_cast<int>(height * aspectRatio);
	}
	int x = (width - w) / 2;
	int y = (height - h) / 2;
	glViewport(x, y, w, h);
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	// Print the debug message
	std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") << "type = " << type << ", severity = " << severity << ", message = " << message << std::endl;
}

namespace core
{
	application::application(int h, int w, const char* name)
	: m_height(h), m_width(w), m_name(name)
	{
		window = std::make_shared <util:: Window > (m_height, m_width, m_name);
		renderer = std::make_shared<Renderer>();
	}

	void application::init()
	{
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			std::cout << "Error!" << std::endl;
		}

#ifdef _DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(MessageCallback, 0);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, NULL, GL_TRUE);
#endif

		// Set the viewport
		int width, height;
		glfwSetFramebufferSizeCallback(window->get(), framebuffer_size_callback);
		glfwGetFramebufferSize(window->get(), &width, &height);
		framebuffer_size_callback(window->get(), width, height);

		renderer->init();
	}

	void application::run()
	{
		init();

		/* Loop until the user closes the window */
		while (!window->shouldClose())
		{
			/* Render here */
			renderer->draw();

			/* Swap front and back buffers */
			glfwSwapBuffers(window->get());

			/* Poll for and process events */
			glfwPollEvents();
		}

		glfwTerminate();
	}

}
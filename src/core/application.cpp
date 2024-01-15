#include <iostream>
#include <GL/glew.h>

#include "application.h"

namespace core
{
	application::application(uint32_t h, uint32_t w, const char* name)
	: m_height(h), m_width(w), m_name(name)
	{
		window = std::make_shared<Window>(m_height, m_width, m_name);
		renderer = std::make_shared<Renderer>();
	}

	void application::run()
	{
		if (glewInit() != GLEW_OK)
		{
			std::cout << "Error!" << std::endl;
		}
		std::cout << glGetString(GL_VERSION) << std::endl;

		/* Loop until the user closes the window */
		while (!window->shouldClose())
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			/* Swap front and back buffers */
			glfwSwapBuffers(window->get());

			/* Poll for and process events */
			glfwPollEvents();
		}

		glfwTerminate();
	}

}
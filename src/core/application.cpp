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


namespace core
{
	application::application(int h, int w, const char* name)
	: m_height(h), m_width(w), m_name(name)
	{
		window = std::make_shared<Window>(m_height, m_width, m_name);
		renderer = std::make_shared<Renderer>(m_height, m_width);
	}

	void application::run()
	{
		if (glewInit() != GLEW_OK)
		{
			std::cout << "Error!" << std::endl;
		}
		
		// Set the viewport
		int width, height;
		glfwSetFramebufferSizeCallback(window->get(), framebuffer_size_callback);
		glfwGetFramebufferSize(window->get(), &width, &height);
		framebuffer_size_callback(window->get(), width, height);

		renderer->init();

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
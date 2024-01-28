#include <iostream>

#include "application.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
	const float targetAspectRatio = 16.0f / 9.0f;
	int viewportWidth = width;
	int viewportHeight = static_cast<int>(viewportWidth / targetAspectRatio);

	if (viewportHeight > height) 
	{
		viewportHeight = height;
		viewportWidth = static_cast<int>(viewportHeight * targetAspectRatio);
	}

	int x = (width - viewportWidth) / 2;
	int y = (height - viewportHeight) / 2;

	glViewport(x, y, viewportWidth, viewportHeight);
}


void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	// Print the debug message
	std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") << "type = " << type << ", severity = " << severity << ", message = " << message << std::endl;
}

Application::Application(int h, int w, const char* name)
: m_height(h), m_width(w), m_name(name), m_deltaTime(0.0f), m_lastFrame(0.0f)
{
	m_window = std::make_shared<Window>(m_height, m_width, m_name);
	m_renderer = std::make_shared<Renderer>();
}

void Application::init()
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error!" << std::endl;
	}

	// Set mouse callback


#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(MessageCallback, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, NULL, GL_TRUE);
#endif
	// Set the viewport
	int width, height;
	glfwSetFramebufferSizeCallback(m_window->get(), framebuffer_size_callback);
	glfwGetFramebufferSize(m_window->get(), &width, &height);
	framebuffer_size_callback(m_window->get(), width, height);

	m_renderer->init();
}

void Application::update()
{
	float currentFrame = glfwGetTime();
	m_deltaTime = currentFrame - m_lastFrame;
	m_lastFrame = currentFrame;

	// Update renderer
	m_renderer->update(m_window, m_deltaTime);

	// Input
	m_processInput();

}

void Application::run()
{
	init();

	/* Loop until the user closes the window */
	while (!m_window->shouldClose())
	{
		/* Update */
		update();

		/* Render here */
		m_renderer->draw(m_window);

		/* Swap front and back buffers */
		glfwSwapBuffers(m_window->get());

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
}

void Application::m_processInput()
{
	//if(glfwGetKey(m_window->get(), GLFW_KEY_W) == GLFW_PRESS)
	//	std::cout << "W" << std::endl;
	//if (glfwGetKey(m_window->get(), GLFW_KEY_S) == GLFW_PRESS)
	//		std::cout << "S" << std::endl;
	//if (glfwGetKey(m_window->get(), GLFW_KEY_A) == GLFW_PRESS)
	//		std::cout << "A" << std::endl;
	//if (glfwGetKey(m_window->get(), GLFW_KEY_D) == GLFW_PRESS)
	//		std::cout << "D" << std::endl;
	// close window
	if (glfwGetKey(m_window->get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_window->get(), true);
}

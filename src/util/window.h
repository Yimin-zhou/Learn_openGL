#pragma once

#include <memory>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

class Window
{
public:
	Window(int height, int width, const char* name);

	void initImGui();
	void drawImGui(uint32_t texture);
	void shutdownImGui();

	bool shouldClose() { return glfwWindowShouldClose(m_window); }
	GLFWwindow* get() { return m_window; }
	glm::ivec2 getSize() { return m_size; };

	void setRenderSize(glm::ivec2 size) { m_renderSize = size; m_renderAspectRatio = (float)size.x / (float)size.y; }
	glm::ivec2 getRenderSize() { return m_renderSize; }
	float getRenderWindowAspectRatio() { return m_renderAspectRatio; }
	void setShouldResize(bool shouldResize) { m_shouldResize = shouldResize; }
	bool shouldResize() { return m_shouldResize; }

private:

	GLFWwindow* m_window;
	glm::ivec2 m_size;

	glm::ivec2 m_renderSize;
	float m_renderAspectRatio;
	bool m_shouldResize = false;
};

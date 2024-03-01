#pragma once

#include "imgui.h"

#include "render/renderer.h"
#include "window.h"


class ImGuiManager
{
public:
	ImGuiManager(std::shared_ptr<Window> window, std::shared_ptr<Renderer> renderer);

	void init();
	void draw(uint32_t texture);
	void shutdown();

	void setRenderSize(glm::ivec2 size) { m_renderSize = size; m_renderAspectRatio = (float)size.x / (float)size.y; }
	glm::ivec2 getRenderSize() { return m_renderSize; }
	float getRenderWindowAspectRatio() { return m_renderAspectRatio; }
	void setShouldResize(bool shouldResize) { m_shouldResize = shouldResize; }
	bool shouldResize() { return m_shouldResize; }

private:
	std::shared_ptr<Window> m_window;
	std::shared_ptr<Renderer> m_renderer;

	glm::ivec2 m_renderSize;
	glm::vec2 m_renderPos;
	float m_renderAspectRatio;
	bool m_shouldResize = false;

	// shader to string
	std::string shaderToString(ShaderName shaderName);

	void drawGizmosLight(std::shared_ptr<Light> light);
	void drawGizmosModel(Model& model);

	// custom imgui widgets
	void modelInspector();

	bool coloredDragFloat3(const char* label, float* values, float speed = 0.01f, float min = 0.0f, float max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	void showTextureSelector(const char* dialogKey, const char* textureName, std::shared_ptr<Texture>& texture);
};
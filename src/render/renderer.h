#pragma once
#include "material/shader.h"
#include "model/model.h"
#include "camera/camera.h"
#include "util/window.h"

#include <string>
#include <vector>

class Renderer
{
public:
	Renderer();

	void init();
	void update(std::shared_ptr<Window> window, float deltaTime);
	void draw(std::shared_ptr<Window> window);

	void resize(std::shared_ptr<Window> window);

	void setCameraAspectRatio(float aspectRatio);
	uint32_t getFinalTexture() { return m_finalTexture; }

public:
	// model path
	std::filesystem::path modelPath = "res/models/cube.obj";

private:
	// properties
	float m_aspectRatio;

	// framebuffers
	uint32_t m_finalFrambufferWidth;
	uint32_t m_finalFrambufferHeight;
	uint32_t m_finalFramebuffer;
	uint32_t m_finalTexture;
	uint32_t m_depthBuffer;

	std::vector<Model> m_models;
	Camera m_camera;
};

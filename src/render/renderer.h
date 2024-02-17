#pragma once
#include <GL/glew.h>

#include "util/window.h"
#include "material/shader.h"
#include "model/model.h"
#include "camera/camera.h"
#include "render/light/LightManager.h"

#include <string>
#include <vector>

class Renderer
{
public:
	Renderer();

	void init();
	void update(std::shared_ptr<Window> window, float deltaTime);
	void draw(glm::vec2 renderSize);

	void resize(glm::vec2 renderSize);

	void setCameraAspectRatio(float aspectRatio);
	uint32_t getFinalTexture() { return m_finalTexture; }

	// camera
	Camera& getCamera() { return m_camera; }

	// lightings
	LightManager& getLightManager() { return m_lightManager; }

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
	ShaderManager m_shaderManager;
	LightManager m_lightManager;
	Camera m_camera;
};

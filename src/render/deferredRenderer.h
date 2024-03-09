#pragma once
#include <GL/glew.h>

#include "window/window.h"
#include "material/shader.h"
#include "model/model.h"
#include "camera/camera.h"
#include "render/light/LightManager.h"
#include "effects/skyBox.h"

#include <string>
#include <vector>

class DeferredRenderer
{
public:
	DeferredRenderer();

	void init();
	void preprocess();
	void update(std::shared_ptr<Window> window, float deltaTime);
	void draw(glm::vec2 renderSize);

	void resize(glm::vec2 renderSize);

	void setCameraAspectRatio(float aspectRatio);
	uint32_t getFinalTexture() { return m_finalTexture; }

	// models
	std::vector<Model>& getModels() { return m_models; }

	// camera
	Camera& getCamera() { return m_camera; }

	// lightings
	LightManager& getLightManager() { return m_lightManager; }

	// skybox
	SkyBox& getSkyBox() { return m_skyBox; }

public:
	// scene model path 
	std::filesystem::path modelPath = "res/models/spaceman/spaceman.obj";

private:
	void createGbuffer();

	void geometryPass();

	// Gbuffer
	uint32_t m_gBuffer;
	uint32_t m_gWorldPositionTex, m_gWorldNormalTex, m_gAlbedoTex;
	uint32_t m_gRoughnessMetalnessAoTex, m_gEmissionTex;
	uint32_t m_depthStencilBuffer;

	// properties
	float m_aspectRatio;
	glm::vec2 m_renderSize;

	// framebuffers
	uint32_t m_finalFrambufferWidth;
	uint32_t m_finalFrambufferHeight;
	uint32_t m_finalFramebuffer;
	uint32_t m_finalTexture;

	// scene
	std::vector<Model> m_models;
	ShaderManager m_shaderManager;
	LightManager m_lightManager;
	Camera m_camera;

	// skybox
	SkyBox m_skyBox;

};

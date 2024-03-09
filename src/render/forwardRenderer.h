#pragma once
#include "renderer.h"

class ForwardRenderer : public Renderer
{
public:
	ForwardRenderer();

	void init() override;
	void preprocess() override;
	void update(std::shared_ptr<Window> window, float deltaTime) override;
	void draw(glm::vec2 renderSize) override;

	void resize(glm::vec2 renderSize) override;

	void setCameraAspectRatio(float aspectRatio) override;
	uint32_t getFinalTexture() override { return m_finalTexture; }

	// models
	std::vector<Model>& getModels() override { return m_models; }

	// camera
	Camera& getCamera() override { return m_camera; }

	// lightings
	LightManager& getLightManager() override { return m_lightManager; }

	// skybox
	SkyBox& getSkyBox() override { return m_skyBox; }

public:
	// scene model path 
	std::filesystem::path modelPath = "res/models/spaceman/spaceman.obj";

private:
	void createFinalBuffer();

	// properties
	float m_aspectRatio;
	glm::vec2 m_renderSize;

	// framebuffers
	uint32_t m_finalFrambufferWidth;
	uint32_t m_finalFrambufferHeight;
	uint32_t m_finalFramebuffer;
	uint32_t m_finalTexture;
	uint32_t m_depthBuffer;

	// scene
	std::vector<Model> m_models;
	ShaderManager m_shaderManager;
	LightManager m_lightManager;
	Camera m_camera;

	// skybox
	SkyBox m_skyBox;

};

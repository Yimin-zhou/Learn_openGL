﻿#pragma once
#include <GL/glew.h>

#include "window/window.h"
#include "material/shader.h"
#include "model/model.h"
#include "camera/camera.h"
#include "render/light/LightManager.h"
#include "effects/skyBox.h"
#include "gBuffer.h"
#include "renderer.h"

#include <string>
#include <vector>

class DeferredRenderer : public Renderer
{
public:

	DeferredRenderer();

	void init() override;
	void preprocess() override;
	void update(std::shared_ptr<Window> window, float deltaTime) override;
	void draw(glm::vec2 renderSize) override;

	void resize(glm::vec2 renderSize) override;

	void setCameraAspectRatio(float aspectRatio) override;
	uint32_t getFinalTexture() override { return m_lightingTexture; }

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
	void createGbuffer();
	void createLightingFramebuffer();

	void geometryPass();
	void lightingPass();

	// quad
	uint32_t m_quadVAO;
	uint32_t m_quadVBO;

	// Gbuffer
	GBuffer m_gBuffer;

	// lighting framebuffer
	uint32_t m_lightingFramebuffer;
	uint32_t m_lightingTexture;

	// properties
	float m_aspectRatio;
	glm::vec2 m_renderSize;

	// scene
	std::vector<Model> m_models;
	ShaderManager m_shaderManager;
	LightManager m_lightManager;
	Camera m_camera;

	// skybox
	SkyBox m_skyBox;

};
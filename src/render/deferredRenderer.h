#pragma once
#include <GL/glew.h>

#include "window/window.h"
#include "material/shader.h"
#include "model/model.h"
#include "camera/camera.h"
#include "render/light/LightManager.h"
#include "render/shadow.h"
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
	std::vector<Model>& getModels() override { return m_models; }
	Camera& getCamera() override { return m_camera; }
	LightManager& getLightManager() override { return m_lightManager; }
	SkyBox& getSkyBox() override { return m_skyBox; }

	const GBuffer& getGBuffer() { return m_gBuffer; }
	uint32_t getShadowMap() { return m_shadow.getShadowMap(); }


protected:
	void createGbuffer();
	void createLightingFramebuffer();

	void shadowPass() override;
	void geometryPass() override;
	void lightingPass() override;
	void forwardPass() override;

	// quad
	uint32_t m_quadVAO;
	uint32_t m_quadVBO;

	// Gbuffer
	GBuffer m_gBuffer;

	// lighting framebuffer
	uint32_t m_lightingFBO;
	uint32_t m_lightingTexture;

	// properties
	float m_aspectRatio;
	glm::vec2 m_renderSize;

	// scene
	std::filesystem::path modelPath;
	std::vector<Model> m_models;
	ShaderManager m_shaderManager;
	LightManager m_lightManager;
	Camera m_camera;

	// skybox
	SkyBox m_skyBox;

	// shadow
	Shadow m_shadow;

};

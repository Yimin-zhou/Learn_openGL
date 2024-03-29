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
#include "scenes/scene.h"

#include <string>
#include <vector>

class DeferredRenderer : public Renderer
{
public:
	DeferredRenderer(std::shared_ptr<Scene> scene);

	void init() override;
	void preprocess() override;
	void update(std::shared_ptr<Window> window, float deltaTime) override;
	void draw(glm::vec2 renderSize) override;
	void resize(glm::vec2 renderSize) override;

	void setCameraAspectRatio(float aspectRatio) override;
	uint32_t getFinalTexture() override { return m_lightingTexture; }
	std::vector<Model>& getModels() override { return m_scene->getModels(); }
	Camera& getCamera() override { return m_scene->getCamera(); }
	LightManager& getLightManager() override { return m_scene->getLightManager(); }
	ShaderManager& getShaderManager() override { return m_scene->getShaderManager(); }

	const GBuffer& getGBuffer() { return m_gBuffer; }
	SkyBox& getSkyBox() override { return m_scene->getSkyBox(); }
	uint32_t getShadowMap() { return m_shadow.getShadowMap(); }

private:
	void createGbuffer();
	void createLightingFramebuffer();

	void shadowPass();
	void geometryPass();
	void lightingPass();

	// scene
	std::shared_ptr<Scene> m_scene;
	
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

	// shadow
	Shadow m_shadow;

};

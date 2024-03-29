#pragma once

#include <memory>
#include <vector>
#include <filesystem>

#include "model/model.h"
#include "camera/camera.h"
#include "render/light/LightManager.h"
#include "effects/skyBox.h"

#include "scene.h"

class SSS : public Scene
{
public:
	SSS();
	void init() override;
	void preprocess() override;
	void update(std::shared_ptr<Window> window, float deltaTime) override;

	Camera& getCamera() { return m_camera; }
	LightManager& getLightManager() { return m_lightManager; }
	ShaderManager& getShaderManager() { return m_shaderManager; }
	std::vector<Model>& getModels() { return m_models; }
	SkyBox& getSkyBox() { return m_skyBox; }
};

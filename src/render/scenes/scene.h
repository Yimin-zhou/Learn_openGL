#pragma once

#include <memory>
#include <vector>
#include <filesystem>
#include "model/model.h"
#include "camera/camera.h"
#include "render/light/LightManager.h"
#include "material/shader.h"
#include "effects/skyBox.h"
#include "window/window.h"

class Scene
{
public:
	Scene() : 
		m_camera(Camera(glm::vec3(0.0f, 1.0f, 2.0f), -90.0f, 0.0f, 3.0f, 0.05f)),
		m_aspectRatio(16.0f / 9.0f),
		m_shaderManager(ShaderManager()),
		m_lightManager(LightManager()),
		m_skyBox(SkyBox()) {}

	virtual ~Scene() = default;

	virtual void init() = 0;
	virtual void update(std::shared_ptr<Window> window, float deltaTime) = 0;
	virtual void preprocess() = 0;

	// Accessor methods can remain non-virtual if they provide general functionality
	// that doesn't change in derived classes.
	Camera& getCamera() { return m_camera; }
	LightManager& getLightManager() { return m_lightManager; }
	ShaderManager& getShaderManager() { return m_shaderManager; }
	std::vector<Model>& getModels() { return m_models; }
	SkyBox& getSkyBox() { return m_skyBox; }

protected:
	std::filesystem::path m_modelPath;
	std::vector<Model> m_models;
	Camera m_camera;
	LightManager m_lightManager;
	ShaderManager m_shaderManager;
	SkyBox m_skyBox;
	float m_aspectRatio;
};

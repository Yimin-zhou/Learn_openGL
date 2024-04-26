
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

class Renderer
{
public:
	virtual ~Renderer() {}

	virtual void init() = 0;
	virtual void preprocess() = 0;
	virtual void update(std::shared_ptr<Window> window, float deltaTime) = 0;
	virtual void draw(glm::vec2 renderSize) = 0;

	virtual void resize(glm::vec2 renderSize) = 0;

	virtual void setCameraAspectRatio(float aspectRatio) = 0;
	virtual uint32_t getFinalTexture() = 0;

	virtual std::vector<Model>& getModels() = 0;

	virtual Camera& getCamera() = 0;

	// lightings
	virtual LightManager& getLightManager() = 0;

	// skybox
	virtual SkyBox& getSkyBox() = 0;

protected:
	virtual void createGbuffer() = 0;
	virtual void createLightingFramebuffer() = 0;
	virtual void shadowPass() = 0;
	virtual void geometryPass() = 0;
	virtual void lightingPass() = 0;
	virtual void forwardPass() = 0;
};

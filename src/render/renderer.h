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

public:
	// model path
	std::filesystem::path modelPath = "res/models/cube.obj";

private:
	std::vector<Model> m_models;
	Camera m_camera;
};

#pragma once
#include "material/shader.h"
#include "model/model.h"

#include <string>
#include <vector>

class Renderer
{
public:
	Renderer();

	void init();
	void draw();

public:
	// model path
	std::filesystem::path modelPath = "res/models/cube.obj";

private:
	std::vector<Model> m_models;
};

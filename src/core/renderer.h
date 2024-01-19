#pragma once
#include "render/shader.h"
#include "render/model.h"

#include <string>
#include <vector>

namespace core
{
	class Renderer
	{
	public:
		Renderer();

		void init();
		void draw();

	private:
		std::vector<render::Model> m_models;
	};
}

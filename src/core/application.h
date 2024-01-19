#pragma once

#include <memory>
#include "renderer.h"

#include "util/window.h"

namespace core
{
	class application
	{
	public:
		std::shared_ptr<util::Window> window;
		std::shared_ptr<Renderer> renderer;

	public:
		application(int h, int w, const char* name);
		void init();
		void run();

	private:
		int m_height;
		int m_width;
		const char* m_name;

	};
}
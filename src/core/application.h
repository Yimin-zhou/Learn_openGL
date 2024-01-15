#pragma once

#include <memory>

#include "window.h"
#include "renderer.h"

namespace core
{
	class application
	{
	public:
		std::shared_ptr<Window> window;
		std::shared_ptr<Renderer> renderer;

	public:
		application(uint32_t h, uint32_t w, const char* name);
		void run();

	private:
		uint32_t m_height;
		uint32_t m_width;
		const char* m_name;

	};
}
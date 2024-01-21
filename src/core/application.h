#pragma once

#include <memory>
#include "render/renderer.h"

#include "util/window.h"

class application
{
public:
	std::shared_ptr<Window> window;
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

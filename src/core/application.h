#pragma once

#include <memory>
#include "render/renderer.h"

#include "util/window.h"

class Application 
{
public:
    Application(int h, int w, const char* name);
    void init();
    void update();
    void run();


private:
    int m_height;
    int m_width;
    const char* m_name;
    std::shared_ptr<Window> m_window;
    std::shared_ptr<Renderer> m_renderer;

    // Input handling methods
    float m_deltaTime, m_lastFrame;
    void m_processInput();
};

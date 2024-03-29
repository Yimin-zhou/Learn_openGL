#pragma once

#include <memory>

#include "render/renderer.h"
#include "render/deferredRenderer.h"
#include "render/forwardRenderer.h"
#include "window/window.h"
#include "window/ImGuiManager.h"

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
    std::shared_ptr<ImGuiManager> m_imGuiManager;
    std::shared_ptr<Scene> m_currentScene;

    // Input handling methods
    float m_deltaTime, m_lastFrame;
    void processInput();
};

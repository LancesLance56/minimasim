#ifndef ENGINE_GUI_H
#define ENGINE_GUI_H

#include "window.h"
#include "imgui/animation_curve.h"
#include "engine.h"

class TestGUI {
public:
    void init(const Window& window, Engine& gameRef);
    void begin_frame();
    void render();

private:
    BezierEditor editor;
    Engine* game = nullptr;  // Pointer to interact with Game instance
};

#endif

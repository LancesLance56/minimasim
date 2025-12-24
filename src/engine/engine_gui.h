#ifndef ENGINE_GUI_H
#define ENGINE_GUI_H

#include "engine.h"
#include "imgui/animation_curve.h"
#include "window.h"

class TestGUI {
public:
    void init(const Window &window, Engine &engineRef);
    void begin_frame() const;
    static void render();

private:
    BezierEditor editor;
    Engine *engine = nullptr; // Pointer to interact with Game instance
};

#endif

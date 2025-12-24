#include "engine_app.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "gfx.h"
#include "input/handle_input.h"
#include "light.h"
#include "window.h"

EngineApp::EngineApp(const std::shared_ptr<Window> &window) : engine_(Engine(window)) {
    engine_.start();
    gui_.init(*engine_.get_window(), engine_);
}

// EngineApp::run now uses the virtual methods of the Scene class
void EngineApp::run(const std::shared_ptr<Scene> &scene) {
    // Scene setup
    const std::shared_ptr<Window> engineWindow = engine_.get_window();

    // Call the setup method
    scene->setup(engine_, engineWindow);

    while (!glfwWindowShouldClose(engineWindow->window)) {

        engine_.calculate_delta_time();

        if (shouldRenderGui) {
            gui_.begin_frame();
            if (scene)
                scene->gui(engine_, engineWindow);
        }

        engine_.update();
        if (scene)
            scene->update(engine_, engineWindow);

        engine_.render();

        if (shouldRenderGui)
            TestGUI::render();

        glfwSwapBuffers(engineWindow->window);
        glfwPollEvents();
    }
}

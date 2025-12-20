#include "./game_app.h"

#include "gfx.h"
#include "lightpos.h"
#include "window.h"
#include "input/handle_input.h"

GameApp::GameApp() {
    game_.init();
    gui_.init(*game_.get_window(), game_);
}

void GameApp::run() {
    Window *gameWindow = game_.get_window();
    while (!glfwWindowShouldClose(gameWindow->window)) {
        game_.calculate_delta_time();

        if (shouldRenderGui)
            gui_.begin_frame();

        glClearColor(SKY_BLUE.red, SKY_BLUE.green, SKY_BLUE.blue, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPolygonMode(GL_FRONT_AND_BACK, polygon_modes[gameWindow->current_polygon_mode_index]);
        glPointSize(4.0f);

        game_.update();
        game_.render();

        if (shouldRenderGui)
            gui_.render();

        glfwSwapBuffers(gameWindow->window);
        glfwPollEvents();

        global_lights_last_frame = global_lights;
    }
}

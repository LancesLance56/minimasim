#ifndef WINDOW_H
#define WINDOW_H

#include "camera.h"
#include "glad_include.h"

enum {
    WINDOW_WIDTH = 800,
    WINDOW_HEIGHT = 600
};

constexpr GLenum polygon_modes[3] = {
    GL_FILL, GL_LINE, GL_POINT
};

class Window {
public:
    unsigned int current_polygon_mode_index = 0;
    GLFWwindow* window;
    Camera* camera; // TODO: Make this a smart pointer, Window.camera is not managed by Window

    Window();

    void set_camera(Camera& camera);
    [[nodiscard]] float  get_aspect_ratio() const;
};

#endif

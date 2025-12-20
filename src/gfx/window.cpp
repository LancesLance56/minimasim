#include "window.h"
#include <iostream>

#include "gfx.h"
#include "glad/glad.h"

namespace {
    unsigned int *current_polygon_mode_index_ptr;
    Camera* current_camera;

    void cursor_position_callback(GLFWwindow* window, double x_pos, double y_pos) {
        if (current_camera == nullptr) {
            std::cout << "Didn't connect a camera to the window\n";
            return;
        }

        current_camera->change_rotation(static_cast<float>(x_pos), static_cast<float>(y_pos), 0.1f);
    }

    void changePolygonMode() {
        if ((*current_polygon_mode_index_ptr) == 2) {
            (*current_polygon_mode_index_ptr) = 0;
        } else {
            (*current_polygon_mode_index_ptr)++;
        }
    }

    void framebuffer_size_callback(GLFWwindow* window, const int width, const int height) {
        glViewport(0, 0, width, height);
    }

    void key_callback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods) {
        if (action == GLFW_PRESS) {
            switch (key) {
                case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, true); break;
                case GLFW_KEY_M: changePolygonMode(); break;
                default: break;
            }
        }
    }
} // namespace

Window::Window() {
    current_polygon_mode_index_ptr = &current_polygon_mode_index;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "engine", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "[ERROR] Failed to create a GLFW window!\n";
        glfwTerminate();
        return;
    }

    // Set the OpenGL context
    glfwMakeContextCurrent(window);

    // Load OpenGL functions with GLAD
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "[ERROR] Failed to initialize GLAD!\n";
        glfwDestroyWindow(window);
        window = nullptr;
        return;
    }

    // Input settings and callbacks
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetKeyCallback(window, key_callback);

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    std::cout << "[INFO] Window and OpenGL initialized.\n";

    camera = new Camera(glm::vec3(-5.0f, 10.0f, 4.5f),
                   glm::vec3(0.0f),
                   glm::vec3(0.0f, 1.0f, 0.0f), static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT);
}


void Window::set_camera(Camera& c) {
    this->camera = &c;
    current_camera = this->camera;
}

float Window::get_aspect_ratio() const {
    int width;
    int height;
    glfwGetWindowSize(window, &width, &height);

    return static_cast<float>(width) / static_cast<float>(height);
}

#ifndef HANDLE_INPUT_H
#define HANDLE_INPUT_H

#include <glm/glm.hpp>

#include "glad_include.h"

inline bool shouldRenderGui = true;

glm::vec3 handle_movement(GLFWwindow* window, glm::vec3 cameraFront, glm::vec3 cameraUp, double deltaTime);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void handle_input(GLFWwindow* window);

#endif

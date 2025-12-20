#ifndef HANDLE_INPUT_H
#define HANDLE_INPUT_H

#include <glm/glm.hpp>

#include "glad_include.h"

inline bool shouldRenderGui = true;

glm::vec3 handleMovement(GLFWwindow *window, glm::vec3 cameraFront, glm::vec3 cameraUp, double deltaTime);
void handleInput(GLFWwindow *window);

#endif

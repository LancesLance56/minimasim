#include "handle_input.h"

#include <iostream>
#include <ostream>

glm::vec3 handle_movement(GLFWwindow *window, glm::vec3 cameraFront, glm::vec3 cameraUp, double deltaTime) {
    glm::vec3 moveDir(0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        moveDir -= cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        moveDir += cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        moveDir += glm::normalize(glm::cross(cameraFront, cameraUp));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        moveDir -= glm::normalize(glm::cross(cameraFront, cameraUp));
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        moveDir += cameraUp;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        moveDir -= cameraUp;

    if (moveDir != glm::vec3(0.0f)) {
        moveDir = glm::normalize(moveDir);
        constexpr float cameraSpeed = 10.0f; // this now works!
        moveDir *= cameraSpeed* static_cast<float>(deltaTime);
    }

    return moveDir;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS) {
        shouldRenderGui = !shouldRenderGui;
    }
}

void handle_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    glfwSetInputMode(window, GLFW_CURSOR, shouldRenderGui ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}


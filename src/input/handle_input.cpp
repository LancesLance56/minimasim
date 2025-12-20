#include "../handle_input.h"

glm::vec3 handleMovement(GLFWwindow *window, glm::vec3 cameraFront, glm::vec3 cameraUp, double deltaTime) {
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
        moveDir *= cameraSpeed * static_cast<float>(deltaTime);
    }

    return moveDir;
}

void handleInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR,  GLFW_CURSOR_HIDDEN);
        shouldRenderGui = false;
    }
    else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        shouldRenderGui = true;
    }
}

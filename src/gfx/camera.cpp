#include "camera.h"

#include <glm/gtx/quaternion.hpp>

/**
 * @brief Creates a perspective projection matrix
 */
Camera::Camera(
    glm::vec3 position,
    glm::vec3 target,
    glm::vec3 global_up,
    float aspect_ratio,
    float fov,
    float near,
    float far
) {
    camera_type = CameraType::Projection3D;
    this->position   = position;
    this->global_up  = global_up;
    this->camera_front = glm::vec3(0.0f, 0.0f, -1.0f);

    m_projection = glm::perspective(glm::radians(fov), aspect_ratio, near, far);
    update_camera();
    rotate_to_target(target);
}

/**
 * @brief Creates an orthographic projection matrix
 */
Camera::Camera(
    glm::vec3 position,
    glm::vec3 target,
    glm::vec3 global_up,
    float left,
    float right,
    float bottom,
    float top,
    float near,
    float far
) {
    camera_type = CameraType::Orthographic;
    this->position   = position;
    this->global_up  = global_up;
    this->camera_front = glm::vec3(0.0f, 0.0f, -1.0f);

    m_projection = glm::ortho(left, right, bottom, top, near, far);
    update_camera();
    rotate_to_target(target);
}

void Camera::change_rotation(float x_pos, float y_pos, float mouse_sensitivity) {
    static float last_x = 0.0f;
    static float last_y = 0.0f;

    if (is_first_frame) {
        last_x = x_pos;
        last_y = y_pos;
        is_first_frame = false;
    }

    float x_offset = (x_pos - last_x) * mouse_sensitivity;
    float y_offset = (last_y - y_pos) * mouse_sensitivity; // reversed
    last_x = x_pos;
    last_y = y_pos;

    yaw   -= x_offset;
    pitch += y_offset;

    // clamp pitch
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // build quaternion from yaw/pitch
    glm::quat q_pitch = glm::angleAxis(glm::radians(pitch), glm::vec3(1, 0, 0));
    glm::quat q_yaw   = glm::angleAxis(glm::radians(yaw),   glm::vec3(0, 1, 0));

    orientation = q_yaw * q_pitch;

    camera_front = glm::normalize(orientation * glm::vec3(0, 0, -1));
}

void Camera::rotate_to_target(glm::vec3 target_vector, float smoothness) {
    glm::vec3 dir = glm::normalize(target_vector - position);
    glm::quat target_orientation = glm::rotation(camera_front, dir);
    orientation = glm::slerp(orientation, target_orientation* orientation, smoothness);
    camera_front = glm::normalize(orientation* glm::vec3(0, 0, -1));
}

void Camera::move_camera(glm::vec3 move_vector) {
    position -= move_vector;
    update_camera();
}

void Camera::update_camera() {
    camera_right = glm::normalize(glm::cross(camera_front, global_up));
    up    = glm::normalize(glm::cross(camera_right, camera_front));

    m_look_at = glm::lookAt(position, position + camera_front, up);
    vp = m_projection * m_look_at * m_model;
}

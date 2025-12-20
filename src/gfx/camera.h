#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>


enum class CameraType {
    Projection3D,
    Orthographic
};


class Camera {
public:
    glm::vec3 camera_front{0.0f, 0.0f, -1.0f};
    glm::vec3 camera_right{1.0f, 0.0f, 0.0f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    glm::vec3 global_up{0.0f, 1.0f, 0.0f};
    glm::quat orientation{1.0f, 0.0f, 0.0f, 0.0f};

    float left = -20.0f;
    float right = 20.0f;
    float top = 20.0f;
    float bottom = -20.0f;

    glm::mat4 vp{1.0f};
    glm::mat4 m_look_at{1.0f};
    glm::mat4 m_projection{1.0f};
    glm::mat4 m_model{1.0f};

    glm::vec3 position{0.0f, 0.0f, 3.0f};

    float yaw   = -90.0f;
    float pitch = 0.0f;

    bool is_first_frame  = true;
    bool is_second_frame = false;

    CameraType camera_type = CameraType::Orthographic;

    Camera() = default;
    Camera(
        glm::vec3 position,
        glm::vec3 target,
        glm::vec3 global_up,
        float aspect_ratio,
        float fov = 90.0f,
        float near = 0.1f,
        float far  = 100.0f
    );
    Camera(
        glm::vec3 position,
        glm::vec3 target,
        glm::vec3 global_up,
        float left,
        float right,
        float bottom,
        float top,
        float near = 0.1f,
        float far  = 100.0f
    );

    // Methods
    void change_rotation(float x_pos, float y_pos, float mouse_sensitivity);
    void rotate_to_target(glm::vec3 target_vector, float smoothness = 1.0f);
    void move_camera(glm::vec3 move_vector);
    void update_camera();
};

#endif

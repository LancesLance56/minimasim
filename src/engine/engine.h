#ifndef GAME_H
#define GAME_H

#include <memory>
#include <vector>

#include "camera.h"
#include "../mesh/cube.h"
#include "../mesh/perlin_plane.h"
#include "renderer.h"
#include "shader.h"
#include "window.h"
#include "../imgui/animation_curve.h"

class Engine {
public:
    Engine();
    void init();
    void update();
    void render();
    void calculate_delta_time();

    [[nodiscard]] Window* get_window() const;
    [[nodiscard]] Camera& get_camera();
    [[nodiscard]] PerlinPlaneSettings& get_settings();
    [[nodiscard]] glm::vec3& get_perlin_position();
    [[nodiscard]] std::vector<BezierKeyframe>& get_curve(); // for GUI to modify

    bool should_draw_normals_geom = false;

private:
    std::unique_ptr<Window> window_;
    std::shared_ptr<Shader> perlin_shader_;
    std::unique_ptr<PerlinPlane> perlin_plane_;
    std::vector<Cube> cubes_;

    Camera camera_;
    Renderer renderer_;

    glm::vec3 perlin_position_ = glm::vec3(0.0f);
    glm::vec3 previous_perlin_position_ = glm::vec3(0.0f);
    PerlinPlaneSettings settings_;
    PerlinPlaneSettings previous_settings_;
    int height_map_lut_resolution_ = 10;

    double delta_time_ = 0.0f;
    double old_time_log_ = 0.0f;

    std::vector<float> height_map_lut_;
    std::vector<BezierKeyframe> curve_;
    std::vector<BezierKeyframe> last_curve_;

    void rebuild_perlin_mesh();
    [[nodiscard]] bool perlin_settings_changed() const;
};

#endif

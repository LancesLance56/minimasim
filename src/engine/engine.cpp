#include "engine.h"

#include "../gfx/lightpos.h"
#include "../input/handle_input.h"

Engine::Engine() : settings_({
        .sizeOfMesh = 100,
        .meshResolution = 1,
        .x_offset = 0.0f,
        .z_offset = 0.0f,
        .scale = 25.0f,
        .seed = 0,
        .heightMultiplier = 5.0f,
        .octaves = 5,
        .persistence = 0.5f,
        .lacunarity = 2.0f
    }), previous_settings_(settings_) {}

void Engine::init() {
    window_ = std::make_unique<Window>();

    camera_ = Camera(
        glm::vec3(-5.0f, 10.0f, 4.5f),
        glm::vec3(static_cast<float>(settings_.sizeOfMesh), 0.0f, static_cast<float>(settings_.sizeOfMesh)),
        glm::vec3(0.0f, 1.0f, 0.0f),
        static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT
    );

    perlin_shader_ = std::make_shared<Shader>(
        "./src/shaders/textured_terrain.vert",
        "./src/shaders/textured_terrain.frag"
    );

    window_->set_camera(camera_);
    glfwSetKeyCallback(window_->window, key_callback);
}

bool Engine::perlin_settings_changed() const {
    return settings_ != previous_settings_ || perlin_position_ != previous_perlin_position_;
}

void Engine::rebuild_perlin_mesh() {
    perlin_plane_ = std::make_unique<PerlinPlane>(
        perlin_shader_,
        settings_,
        height_map_lut_,
        perlin_position_
    );
}

void Engine::update() {
    handle_input(window_->window);

    for (const auto& [position, color] : global_lights) {
        cubes_.emplace_back(position, glm::vec3(0.0f), 1, color);
    }

    // Update camera
    camera_.move_camera(handle_movement(window_->window, camera_.camera_front, camera_.global_up, delta_time_));
    camera_.update_camera();

    bool needRebuild = false;

    if (perlin_settings_changed()) {
        needRebuild = true;
    }

    if (curve_ != last_curve_ && glfwGetMouseButton(window_->window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
        height_map_lut_ = BezierEditor::computeHeightRemapLUT(curve_, height_map_lut_resolution_);
        last_curve_ = curve_;
        needRebuild = true;
    }

    if (needRebuild) {
        rebuild_perlin_mesh();
        previous_settings_ = settings_;
        previous_perlin_position_ = perlin_position_;
    }
}


void Engine::render() {
    if (perlin_plane_) perlin_plane_->draw(camera_.mvp, camera_, should_draw_normals_geom);
    for (const auto &cube: cubes_) cube.draw(camera_.mvp, camera_);
    cubes_.clear();
}

void Engine::calculate_delta_time() {
    delta_time_ = glfwGetTime() - old_time_log_;
    old_time_log_ = glfwGetTime();
}

Window *Engine::get_window() const {
    return window_.get();
}

Camera &Engine::get_camera() {
    return camera_;
}

PerlinPlaneSettings &Engine::get_settings() {
    return settings_;
}

glm::vec3 &Engine::get_perlin_position() {
    return perlin_position_;
}

std::vector<BezierKeyframe> &Engine::get_curve() {
    return curve_;
}

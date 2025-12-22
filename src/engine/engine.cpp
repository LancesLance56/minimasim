#include "engine.h"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <utility>

#include "../gfx/light.h"
#include "../input/handle_input.h"
#include "mesh/cube_entity.h"
#include "mesh/sphere_entity.h"
#include "precompiled_entities.h"
#include "precompiled_shaders.h"
#include "window.h"

Engine::Engine(std::shared_ptr<Window> window) {
    delta_time = 0.0f;
    this->window = std::move(window);
    precompiled_shaders::compile();
    precompiled_entities::compile();
}

void Engine::start() {
    render_settings = { false, false, false };

    camera = Camera(
        glm::vec3(-5.0f, 10.0f, 4.5f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT)
    );
    light_render_objects = std::vector<LightRenderObject>();

    window->set_camera(camera);
    glfwSetKeyCallback(window->window, key_callback);

    old_time_log = glfwGetTime();
}

void Engine::update() {
    handle_input(window->window);

    if (!shouldRenderGui) {
        camera.move_camera(handle_movement(window->window, camera.camera_front, camera.global_up, delta_time));
        camera.update_camera();
    }

    registry.update_systems(delta_time);
}

void Engine::render() {
    glClearColor(background_rgb.x, background_rgb.y, background_rgb.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::vector<Light> lights;
    std::ranges::transform(
            light_render_objects, std::back_inserter(lights),
            [](const LightRenderObject &t){ return t.light; });

    registry.draw_systems(camera, render_settings, lights);

    for (const auto &[light, visible, radius] : light_render_objects) {
        if (!visible) continue;

        precompiled_entities::white_sphere_entity->transform.position = light.position;
        precompiled_entities::white_sphere_entity->radius = radius;
        precompiled_entities::white_sphere_entity->draw(camera, render_settings, lights);
    }
}

void Engine::calculate_delta_time() {
    double current_time = glfwGetTime();
    delta_time = current_time - old_time_log;
    old_time_log = current_time;
}

std::shared_ptr<Window> Engine::get_window() const {
    return window;
}

Camera& Engine::get_camera() {
    return camera;
}

void Engine::add_light(LightRenderObject light_ro) {
    light_render_objects.emplace_back(light_ro);
}

void Engine::set_light(const std::vector<LightRenderObject> &l) {
    this->light_render_objects = l;
}

double Engine::get_delta_time() const {
    return delta_time;
}
#ifndef GAME_H
#define GAME_H

#include <memory>
#include <vector>

#include "../../include/glm/detail/type_vec3.hpp"
#include "camera.h"
#include "engine/render_settings.h"
#include "entity_registry.h"
#include "light.h"

class Window;

class Engine {
public:
    explicit Engine(std::shared_ptr<Window> window);
    void start();
    void update();
    void render();
    void calculate_delta_time();

    [[nodiscard]] std::shared_ptr<Window> get_window() const;
    Camera &get_camera();

    void set_light(const std::vector<LightRenderObject> &l);
    void add_light(LightRenderObject light_ro);

    [[nodiscard]] double get_delta_time() const;

    RenderSettings render_settings;
    std::vector<LightRenderObject> light_render_objects;

    EntityRegistry registry;

    glm::vec3 background_rgb = glm::vec3(0.12f);

    std::shared_ptr<Window> window;
    Camera camera;
    double delta_time;
    double old_time_log = 0.0f;
};

#endif

#ifndef GAME_H
#define GAME_H

#include <memory>
#include <vector>

#include "entity/entity.h"

#include "camera.h"
#include "entity/components/mesh_renderer_component.h"
#include "glm/detail/_vectorize.hpp"
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
    Camera& get_camera();

    void add_entity(const std::shared_ptr<EntityBase>& entity); // <- shared_ptr

    void set_light(const std::vector<LightRenderObject> &l);
    void add_light(LightRenderObject light_ro);

    [[nodiscard]] double get_delta_time() const;

    RenderSettings render_settings;
    std::vector<LightRenderObject> light_render_objects;
    std::vector<std::shared_ptr<EntityBase>> entities; // <- changed from unique_ptr
    glm::vec3 background_rgb = glm::vec3(0.12f);

    std::shared_ptr<Window> window;
    Camera camera;
    double delta_time;
    double old_time_log = 0.0f;
};



#endif

#include "scene_gravity.h"
#include "../engine/engine.h"
#include "../mesh/sphere_entity.h"
#include "app/engine_app.h"
#include "light.h"

#include "engine/entity/components/trail_component.h"
#include "gfx.h"
#include "math/n_body_sim.h"

class AxisGravityScene : public Scene {
public:
    AxisGravityScene() = default;

    void setup(Engine& engine, std::shared_ptr<Window> window) override {
        //float window_aspect_ratio = engine.window->get_aspect_ratio();
        //engine.camera = Camera(
        //    glm::vec3(0.0f, 15.0f, 0.0f), glm::vec3(0.0f),
        //    glm::vec3(0.0f, 1.0f, 0.0f), -15.0f * window_aspect_ratio,
        //    15.0f * window_aspect_ratio, -15.0f, 15.0f, -100.0f, 100.0f);

        blue_sphere = std::make_shared<SphereEntity>(
            glm::vec3(8.0f, 0.0f, -8.0f), 3, 1, false, blue_base_material);
        orange_sphere = std::make_shared<SphereEntity>(
            glm::vec3(-8.0f, 0.0f, 8.0f), 3, 1, false, orange_base_material);
        yellow_sphere = std::make_shared<SphereEntity>(
            glm::vec3(0.0f), 3, 1, false, yellow_base_material);

        engine.add_entity(blue_sphere);
        engine.add_entity(yellow_sphere);
        engine.add_entity(orange_sphere);
        engine.set_light(lights);

        blue_sphere->add_component<NBodySim::MassComponent>(glm::vec3(0.0f, 0.0f, 1.0f), 5.0f);
        orange_sphere->add_component<NBodySim::MassComponent>(glm::vec3(1.0f, 0.0f, 1.0f), 5.0f);
        yellow_sphere->add_component<NBodySim::MassComponent>(glm::vec3(0.0f, 0.0f, 0.0f), 5.0f);
        blue_sphere->add_component<TrailComponent3D>(200);
    }

    void update(Engine& engine, std::shared_ptr<Window> window) override {
        blue_sphere->get_component<TrailComponent3D>().append_trail(blue_sphere->transform.position);

        NBodySim::integrate_system_rk4(
                1.0f, static_cast<float>(engine.delta_time) * step_speed,
                blue_sphere->get_component<NBodySim::MassComponent>(),
                orange_sphere->get_component<NBodySim::MassComponent>(),
                yellow_sphere->get_component<NBodySim::MassComponent>());

        engine.light_render_objects[0].light.position = (orange_sphere->transform.position + blue_sphere->transform.position + yellow_sphere->transform.position) / 3.0f;
    }

private:
    std::shared_ptr<SphereEntity> blue_sphere;
    std::shared_ptr<SphereEntity> orange_sphere;
    std::shared_ptr<SphereEntity> yellow_sphere;

    std::vector<LightRenderObject> lights = {
        { { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), 1.0f }, false, 2.5f }
    };

    float step_speed = 5.0f;
};

std::shared_ptr<Scene> make_scene_gravity() {
    return std::make_shared<AxisGravityScene>();
}
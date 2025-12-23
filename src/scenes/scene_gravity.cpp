#include "scene_gravity.h"
#include "../engine/engine.h"
#include "../mesh/sphere_entity.h"
#include "app/engine_app.h"
#include "light.h"
#include "gfx.h"
#include "math/n_body_sim.h"

class SolarSystemScene : public Scene {
public:
    SolarSystemScene() = default;

    void setup(Engine& engine, std::shared_ptr<Window> window) override {
        sun = engine.registry.create_entity<SphereEntity>(
            glm::vec3(0.0f), 3, 1.0f, false, yellow_base_material);
        earth = engine.registry.create_entity<SphereEntity>(
            glm::vec3(15.0f, 0.0f, 0.0f), 3, 1.0f, true, blue_base_material);
        mars = engine.registry.create_entity<SphereEntity>(
            glm::vec3(-25.0f, 0.0f, 0.0f), 3, 0.8f, true, orange_base_material);

        sun->add_component<NBodySim::MassComponent>(glm::vec3(0.0f), 1000.0f);
        earth->add_component<NBodySim::MassComponent>(glm::vec3(0.0f, 0.0f, 8.16f), 1.0f);
        mars->add_component<NBodySim::MassComponent>(glm::vec3(0.0f, 0.0f, -6.32f), 0.6f);

        engine.set_light(lights);
    }

    void update(Engine& engine, std::shared_ptr<Window> window) override {
        float dt = static_cast<float>(engine.delta_time) * step_speed;

        auto& sun_phys = sun->get_component<NBodySim::MassComponent>();
        auto& earth_phys = earth->get_component<NBodySim::MassComponent>();
        auto& mars_phys = mars->get_component<NBodySim::MassComponent>();

        NBodySim::integrate_system_rk4(1.0f, dt, sun_phys, earth_phys, mars_phys);

        sun->transform.position = sun_phys.center_of_mass;
        earth->transform.position = earth_phys.center_of_mass;
        mars->transform.position = mars_phys.center_of_mass;

        lights[0].light.position = sun->transform.position;
    }

private:
    SphereEntity* sun = nullptr;
    SphereEntity* earth = nullptr;
    SphereEntity* mars = nullptr;

    float step_speed = 1.0f;
    std::vector<LightRenderObject> lights = {
        { { glm::vec3(0.0f), glm::vec3(1.0f), 1.5f }, false, 60.0f }
    };
};

std::shared_ptr<Scene> make_scene_gravity() {
    return std::make_shared<SolarSystemScene>();
}
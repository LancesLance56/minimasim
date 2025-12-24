#include "scene_gravity.h"
#include "../engine/engine.h"
#include "../mesh/sphere_entity.h"
#include "app/engine_app.h"
#include "gfx.h"
#include "light.h"
#include "math/n_body_sim.h"

#include <fstream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class SolarSystemScene : public Scene {
public:
    SolarSystemScene() = default;

    void setup(Engine &engine, std::shared_ptr<Window> window) override {
        std::ifstream file("solar_system_data.json");
        if (!file.is_open()) return;

        json data;
        try {
            file >> data;
        } catch (const json::parse_error& e) {
            return;
        }

        if (!data.is_array()) return;

        for (const auto& item : data) {
            // Use .value() to provide defaults and avoid "Assertion Failed"
            double mass = item.value("mass", 0.0);
            float radius = item.value("radius", 0.1f);
            bool is_emissive = item.value("is_emissive", false);

            // For arrays, check contains() or size() before access to prevent asserts
            if (!item.contains("position") || !item.contains("velocity") || !item.contains("color")) {
                continue;
            }

            glm::dvec3 pos(item["position"][0], item["position"][1], item["position"][2]);
            glm::dvec3 vel(item["velocity"][0], item["velocity"][1], item["velocity"][2]);
            glm::vec3 col(item["color"][0], item["color"][1], item["color"][2]);

            Material mat;
            if (is_emissive) {
                mat.ambient = col * 2.0f;
                mat.diffuse = col;
                mat.specular = glm::vec3(0.0f);
            } else {
                mat.ambient = col * 0.02f;
                mat.diffuse = col;
                mat.specular = glm::vec3(0.2f);
            }

            auto* entity = engine.registry.create_entity<SphereEntity>(
                glm::vec3(pos), 3, radius, !is_emissive, mat
            );

            entity->add_component<NBodySim::MassComponent>(vel, mass);
            entities.push_back(entity);
        }

        for (auto* e : entities) {
            physics_bodies.push_back(&e->get_component<NBodySim::MassComponent>());
        }

        if (!entities.empty()) {
            lights[0].light.position = glm::vec3(entities[0]->transform.position);
            lights[0].light.intensity = 100.0f;
        }
        engine.set_light(lights);
    }

    void update(Engine &engine, std::shared_ptr<Window> window) override {
        double frame_time = engine.delta_time * time_scale;
        if (frame_time > 0.25) frame_time = 0.25;
        accumulator += frame_time;

        constexpr float fixed_dt = 0.01f;
        constexpr double G_GAUSSIAN = 0.0002959122;

        while (accumulator >= fixed_dt) {
            NBodySim::integrate_system_rk4(G_GAUSSIAN, fixed_dt, physics_bodies);
            accumulator -= fixed_dt;
        }

        for (size_t i = 0; i < entities.size(); ++i) {
            entities[i]->transform.position = glm::vec3(physics_bodies[i]->center_of_mass);
        }

        if (!entities.empty()) {
            lights[0].light.position = glm::vec3(entities[0]->transform.position);
        }
    }

private:
    std::vector<SphereEntity*> entities;
    std::vector<NBodySim::MassComponent*> physics_bodies;
    double accumulator = 0.0;
    float time_scale = 30.0f;
    std::vector<LightRenderObject> lights = {{{glm::vec3(0.0f), glm::vec3(1.0f), 100.0f}, false, 60.0f}};
};

std::shared_ptr<Scene> make_scene_gravity() { return std::make_shared<SolarSystemScene>(); }
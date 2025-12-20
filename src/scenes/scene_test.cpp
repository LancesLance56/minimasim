#include "scene_test.h"

#include "mesh/sphere_entity.h"

class TestScene : public Scene {
    std::shared_ptr<SphereEntity> blue_sphere;
    std::shared_ptr<SphereEntity> orange_sphere;
    std::vector<LightRenderObject> lights = {
        { { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), 1.0f }, true, 2.5f }
    };

    void setup(Engine& engine, std::shared_ptr<Window> window) override {
        blue_sphere = std::make_shared<SphereEntity>(
            glm::vec3(8.0f, 0.0f, -8.0f), 3, 1, false, blue_base_material);
        orange_sphere = std::make_shared<SphereEntity>(
            glm::vec3(-8.0f, 0.0f, 8.0f), 3, 1, false, orange_base_material);

        engine.add_entity(blue_sphere);
        engine.add_entity(orange_sphere);
        engine.set_light(lights);
    }
};

std::shared_ptr<Scene> make_scene_test() {
    return std::make_shared<TestScene>();
}
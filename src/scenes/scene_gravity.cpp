#include "../engine/engine.h"
#include "app/engine_app.h"
#include "light.h"
#include "../mesh/sphere_entity.h"
#include "scene_axis_cube.h" // Include the header

// Define the concrete scene class
class AxisCubeScene : public Scene {
public:
    AxisCubeScene() {
        // Initialize lights in the constructor
        lights = {
            { { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), 1.0f }, true, 2.5f }
        };
    }

    void setup(Engine& engine, std::shared_ptr<Window> window) override {
        // Camera setup
        engine.camera.position = glm::vec3(0.0f, 15.0f, 0.0f);
        engine.camera.rotate_to_target(glm::vec3(0.0f, 0.0f, 0.0f));

        // Create spheres and assign to class members.
        blue_sphere = std::make_shared<SphereEntity>(
            glm::vec3(4.0f, 0.0f, -4.0f), 3, 1, true, blue_base_material);
        orange_sphere = std::make_shared<SphereEntity>(
            glm::vec3(-4.0f, 0.0f, 4.0f), 3, 1, true, orange_base_material);

        // Add to engine
        engine.set_light(lights);
        engine.add_entity(blue_sphere);
        engine.add_entity(orange_sphere);
    }

    void update(Engine& engine, std::shared_ptr<Window> window) override {
        // Access class members directly; no risk of dangling references.
        if (blue_sphere && orange_sphere) {
            // simple animation: orbit around origin
            static float time = 0.0f;
            time += static_cast<float>(engine.get_delta_time()) * 1.0f;

            blue_sphere->position.x = 6.0f * cos(time);
            blue_sphere->position.z = 6.0f * sin(time);

            orange_sphere->position.x = -6.0f * cos(time);
            orange_sphere->position.z = -6.0f * sin(time);
        }
    }

private:
    // Scene state (entities and lights) are now private class members.
    std::shared_ptr<SphereEntity> blue_sphere;
    std::shared_ptr<SphereEntity> orange_sphere;
    std::vector<LightRenderObject> lights;
};

// Factory function now returns a shared pointer to the new class
std::shared_ptr<Scene> make_axis_cube_scene() {
    return std::make_shared<AxisCubeScene>();
}
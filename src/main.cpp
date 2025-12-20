#include "app/engine_app.h"
#include "scenes/scene_gravity.h"

int main() {
    if (!glfwInit()) {
        std::cerr << "[ERROR] Failed to initialize GLFW!\n";
    }

    const auto window = std::make_shared<Window>();
    auto app = EngineApp(window);
    const auto scene = make_scene_gravity();
    app.run(scene);
    return 0;
}

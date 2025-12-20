#ifndef ENGINE_APP_H
#define ENGINE_APP_H

#include "../engine/engine.h"
#include "../engine/engine_gui.h"
#include <functional>
#include <memory> // For std::shared_ptr

// A Scene is now an abstract base class
class Scene {
public:
    // Lifecycle methods for concrete scenes to implement
    virtual void setup(Engine& engine, std::shared_ptr<Window> window) = 0;

    // Optional methods with default (empty) implementation
    virtual void update(Engine& engine, std::shared_ptr<Window> window) {}
    virtual void gui(Engine& engine, std::shared_ptr<Window> window) {}

    // Virtual destructor is crucial for proper cleanup
    virtual ~Scene() = default;
};

class EngineApp {
public:
    EngineApp(const std::shared_ptr<Window>& window);

    // Run now accepts a shared_ptr to a Scene instance
    void run(const std::shared_ptr<Scene>& scene);

private:
    Engine engine_;   // Core engine
    TestGUI gui_;     // GUI wrapper
};

#endif // ENGINE_APP_H
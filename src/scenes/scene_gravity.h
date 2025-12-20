#ifndef SCENE_AXIS_CUBE_H
#define SCENE_AXIS_CUBE_H

#include <memory>
#include "app/engine_app.h"

// Forward declaration for the concrete scene class
class AxisGravityScene;

// Factory function now returns a shared_ptr to the base Scene class
std::shared_ptr<Scene> make_scene_gravity();

#endif // SCENE_AXIS_CUBE_H
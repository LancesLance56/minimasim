#ifndef ENTITY_BASE_H
#define ENTITY_BASE_H

#include <span>
#include <iostream>

#include "camera.h"
#include "engine/render_settings.h"
#include "light.h"

class EntityBase {
public:
    // Virtual destructor is critical for unique_ptr cleanup in the Scene
    virtual ~EntityBase() = default;

    virtual void update(double dt) = 0;

    // Default implementations are empty to allow optional overriding
    virtual void draw(const Camera &camera,
                      const RenderSettings &settings,
                      const std::span<const Light> &lights) {}

    virtual void draw(const Camera &camera,
                      const RenderSettings &settings) {}

    virtual void on_gui(int entity_id) {}

    virtual RenderSettings& get_render_settings() = 0;

    // Optimization: Pass by const reference to avoid copying large structs
    virtual void set_render_settings(const RenderSettings& rs) = 0;
};

#endif
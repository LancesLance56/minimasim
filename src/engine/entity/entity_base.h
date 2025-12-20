#ifndef ENTITY_BASE_H
#define ENTITY_BASE_H

#include <span>

#include "camera.h"
#include "engine/render_settings.h"
#include "light.h"

class EntityBase {
public:
    virtual ~EntityBase() = default;

    virtual void update(double dt) = 0;
    virtual void draw(const Camera &camera,
                      const RenderSettings &settings,
                      const std::span<const Light> &lights) { std::cout << "called entity_base::draw" << std::endl; }
    virtual void draw(const Camera &camera,
                      const RenderSettings &settings) { std::cout << "called entity_base::draw " << std::endl; }
    virtual void on_gui(int entity_id) {}

    virtual RenderSettings& get_render_settings() = 0;
    virtual void set_render_settings(RenderSettings rs) = 0;
};

#endif

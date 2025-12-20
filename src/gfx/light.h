#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <span>

#include "shader.h"

struct Light {
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
};

struct LightRenderObject {
    Light light = { glm::vec3(0.0f), glm::vec3(1.0f), 1.0f };
    bool visible = true;
    float radius = 1.0f;
};

namespace lightutils {
    void apply_lights_to_shader(const Shader& shader, std::span<const Light> light_entities);
}

#endif

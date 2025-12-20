#include "light.h"

#include <format>

namespace lightutils {
    void apply_lights_to_shader(const Shader& shader, const std::span<const Light> light_entities) {
        shader.set_int("lightCount", static_cast<int>(light_entities.size()));
        for (int i = 0; i < light_entities.size(); ++i) {
            shader.set_vec3(std::format("lights[{}].position", i), light_entities[i].position);
            shader.set_vec3(std::format("lights[{}].color", i), light_entities[i].color);
            shader.set_float(std::format("lights[{}].intensity", i), light_entities[i].intensity);
        }
    }
}

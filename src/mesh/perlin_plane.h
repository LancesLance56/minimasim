#ifndef PERLIN_PLANE_H
#define PERLIN_PLANE_H

#include <vector>
#include <glm/glm.hpp>

#include "../engine/entity/components/mesh_filter_component.h"
#include "../engine/entity/entity.h"
#include "math/perlin.h"
#include "shader.h"

class Camera;

class PerlinPlane {
public:
    glm::vec3 position_ = glm::vec3(0.0f);
    PerlinPlane() = default;
    PerlinPlane(const std::shared_ptr<Shader>& shader,
                const PerlinPlaneSettings& settings,
                const std::vector<float>& lut,
                const glm::vec3& position = glm::vec3(0.0f));

    void draw(const Camera& camera, const RenderSettings& render_settings, std::span<const Light> light_entities);

private:
    MeshFilter mesh_;
    MeshRenderer mesh_normals_renderer_;
    std::vector<GLuint> terrain_texture_ids_;
    std::vector<float> lut_;
};

#endif // PERLIN_PLANE_H

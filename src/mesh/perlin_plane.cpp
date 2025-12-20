#include "perlin_plane.h"
#include "../math/perlin_noise_map.h"
#include <glm/gtc/matrix_transform.hpp>
#include <numeric>
#include <iostream>

namespace {
    void bind_terrain_textures(const GLuint shader_program,
                               const std::vector<GLuint>& texture_ids,
                               const std::string &uniform_name)
    {
        glUseProgram(shader_program);
        for (size_t i = 0; i < texture_ids.size(); ++i) {
            glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(i));
            glBindTexture(GL_TEXTURE_2D, texture_ids[i]);
        }
        GLint location = glGetUniformLocation(shader_program, uniform_name.c_str());
        if (location == -1) return;
        std::vector<GLint> units(texture_ids.size());
        std::iota(units.begin(), units.end(), 0);
        glUniform1iv(location, static_cast<GLsizei>(texture_ids.size()), units.data());
    }

    float hermite(float v0, float v1, float m0, float m1, float t) {
        float t2 = t* t;
        float t3 = t2 * t;
        return (2*t3 - 3*t2 + 1) * v0 + (t3 - 2*t2 + t) * m0 + (-2*t3 + 3*t2) * v1 + (t3 - t2) * m1;
    }

    float remap_height(float h, const std::vector<float>& lut, int resolution) {
        if (resolution < 2 || lut.size() < static_cast<size_t>(resolution)) return h;

        auto idx = static_cast<int>(std::floor(h * static_cast<float>(resolution - 1)));
        auto t = h * (static_cast<float>(resolution) - 1) - static_cast<float>(idx);

        if (idx < 0) return lut[0];
        if (idx >= resolution - 1) return lut[resolution - 1];

        auto scale = static_cast<float>(resolution - 1);
        auto m0 = (idx == 0) ? (lut[1] - lut[0]) * scale
                               : (lut[idx+1] - lut[idx-1]) * (scale* 0.5f);
        auto m1 = (idx+1 == resolution-1) ? (lut[idx+1] - lut[idx]) * scale
                                           : (lut[idx+2] - lut[idx]) * (scale* 0.5f);
        return hermite(lut[idx], lut[idx+1], m0, m1, t);
    }
}

PerlinPlane::PerlinPlane(
    const std::shared_ptr<Shader> &shader,
    const PerlinPlaneSettings &settings,
    const std::vector<float> &lut,
    const glm::vec3 &position)
    : position_(position)
{
    PerlinNoiseMap perlinGen(settings);

    std::vector<float> vertices = perlinGen.vertices;
    constexpr int floats_per_vertex = 8;
    for (size_t i = 0; i < vertices.size(); i += floats_per_vertex) {
        float height = vertices[i + 1];
        vertices[i + 1] = remap_height(height, lut, static_cast<int>(lut.size())) * settings.heightMultiplier;
    }

    perlinGen.recalculate_vertex_normals(vertices, perlinGen.indices, settings);

    // Create Mesh with optional normals (COLOR_TEXTURE_NORMALS)
    mesh_ = MeshFilter(
        vertices,
        perlinGen.indices,
        shader,
        glm::vec3(1.0f),
        std::nullopt,  // no single texture
        true           // normals included
    );

    // Load terrain textures
    const std::vector<std::string> terrain_texture_paths = {
        "./src/textures/Water.png",
        "./src/textures/Sandy grass.png",
        "./src/textures/Grass.png",
        "./src/textures/Rocks 1.png",
        "./src/textures/Rocks 2.png",
        "./src/textures/Stony ground.png",
        "./src/textures/Snow.png"
    };

    terrain_texture_ids_.clear();
    for (const auto &path : terrain_texture_paths) {
        GLuint texID = create_opengl_texture(path);
        terrain_texture_ids_.push_back(texID);
    }
}

void PerlinPlane::draw(
    const Camera &camera,
    const RenderSettings &render_settings,
    const std::span<const Light> light_entities)
{
    bind_terrain_textures(mesh_.renderer.shader->ID, terrain_texture_ids_, "terrainTextures");

    glm::mat4 model = glm::translate(glm::mat4(1.0f), position_);
    const glm::mat4 mvp = camera.vp* model;

    mesh_.renderer.model = model;
    mesh_.renderer.draw(mvp, camera, light_entities);

    if (render_settings.draw_normals) {
        mesh_normals_renderer_.model = model;
        mesh_normals_renderer_.draw(mvp, camera, light_entities);
    }
}

#include "mesh_renderer_component.h"
#include <format>
#include <iostream>

#include "camera.h"
#include "math/math_tools.h"
#include "texture.h"

MeshRenderer::MeshRenderer(const std::shared_ptr<Shader>& shader_ptr, const MeshFilter& mesh)
    :   vao_(VAO()),
        vbo_(BO<BufferType::VBO>(mesh.mesh_data.bo_usage_hint)),
        ebo_(BO<BufferType::EBO>(mesh.mesh_data.bo_usage_hint)),
        mesh_filter(mesh),
        shader(shader_ptr),
        vertices_count(mesh.mesh_data.vertices.size()),
        indices_count(mesh.mesh_data.indices.size())
    {

    vao_.bind();
    vbo_.set_data(VectorUtils::flatten_container(mesh.mesh_data.vertices));

    if (!mesh.mesh_data.indices.empty()) {
        ebo_.set_data(mesh.mesh_data.indices);
        should_draw_elements_ = true;
    }

    bool has_texture = mesh.mesh_data.flags == Flags::PosTexture || mesh.mesh_data.flags == Flags::PosNormalsTex;
    bool has_normals = mesh.mesh_data.flags == Flags::PosNormals || mesh.mesh_data.flags == Flags::PosNormalsTex;

    // Set vertex attributes
    int stride = 3 * sizeof(float); // Default: positions only
    if (has_texture && has_normals) {
        stride = 8 * sizeof(float);
        vao_.vattribptr(0, 3, GL_FLOAT, stride, 0);                 // position
        vao_.vattribptr(1, 2, GL_FLOAT, stride, 3 * sizeof(float)); // texcoord
        vao_.vattribptr(2, 3, GL_FLOAT, stride, 5 * sizeof(float)); // normal
    } else if (has_texture) {
        stride = 5 * sizeof(float);
        vao_.vattribptr(0, 3, GL_FLOAT, stride, 0);
        vao_.vattribptr(1, 2, GL_FLOAT, stride, 3 * sizeof(float));
    } else if (has_normals) {
        stride = 6 * sizeof(float);
        vao_.vattribptr(0, 3, GL_FLOAT, stride, 0);
        vao_.vattribptr(1, 3, GL_FLOAT, stride, 3 * sizeof(float));
    } else {
        // Positions only (e.g., for trails)
        vao_.vattribptr(0, 3, GL_FLOAT, stride, 0);
    }

    // Texture
    if (has_texture && mesh.mesh_data.texture_path && !mesh.mesh_data.texture_path->empty()) {
        texture = create_opengl_texture(*mesh.mesh_data.texture_path);
        if (texture == 0) std::cerr << "[MeshRenderer] Failed to create texture\n";
    } else {
        texture = -1;
    }
    VAO::unbind();
}

void MeshRenderer::draw(const glm::mat4& mvp, const Camera& camera,
                    std::span<const Light> lights, const Material& material) const {

    shader->use();
    shader->set_mat4("mvp", mvp);
    shader->set_mat4("model", model);
    shader->set_vec3("viewPos", camera.position);
    shader->set_material("material", mesh_filter.mesh_data.material);

    lightutils::apply_lights_to_shader(*shader, lights);
    material.apply_to_shader(*shader);

    vao_.bind();
    if (texture != 0 && texture != -1) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader->set_int("baseTexture", 0);
    }

    if (should_draw_elements_) {
        glDrawElements(static_cast<GLenum>(mesh_filter.mesh_data.draw_mode), mesh_filter.mesh_data.indices.size(), GL_UNSIGNED_INT, nullptr);
    } else {
        int normals_count = mesh_filter.mesh_data.flags == Flags::PosNormalsTex || mesh_filter.mesh_data.flags == Flags::PosNormals ? 3 : 0;
        int tex_count = mesh_filter.mesh_data.flags == Flags::PosNormalsTex || mesh_filter.mesh_data.flags == Flags::PosTexture ? 2 : 0;
        glDrawArrays(static_cast<GLenum>(mesh_filter.mesh_data.draw_mode), 0, mesh_filter.mesh_data.vertices.size() / (3 + normals_count + tex_count));
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void MeshRenderer::draw(const glm::mat4& mvp, const Camera& camera,
                    std::span<const Light> lights) const {
    shader->use();
    shader->set_mat4("mvp", mvp);
    shader->set_mat4("model", model);
    shader->set_vec3("viewPos", camera.position);
    shader->set_material("material", mesh_filter.mesh_data.material);

    shader->set_int("lightCount", static_cast<int>(lights.size()));
    for (int i = 0; i < lights.size(); ++i) {
        shader->set_vec3(std::format("lights[{}].position", i), lights[i].position);
        shader->set_vec3(std::format("lights[{}].color", i), lights[i].color);
        shader->set_float(std::format("lights[{}].intensity", i), lights[i].intensity);
    }

    vao_.bind();
    if (texture != 0 && texture != -1) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader->set_int("baseTexture", 0);
        shader->set_int("shouldDrawBaseTexture", 1);
    } else {
        shader->set_int("shouldDrawBaseTexture", 0);
    }

    // FIX: Use the dynamic draw mode
    if (should_draw_elements_) {
        glDrawElements(static_cast<GLenum>(mesh_filter.mesh_data.draw_mode), mesh_filter.mesh_data.indices.size(), GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawArrays(static_cast<GLenum>(mesh_filter.mesh_data.draw_mode), 0, mesh_filter.mesh_data.indices.size());
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}
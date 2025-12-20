#include "mesh_component.h"
#include <format>
#include <iostream>

Renderer::Renderer(const std::shared_ptr<Shader>& shader_ptr, const MeshData& data)
    : vao_(VAO()),
      vbo_(BO<BufferType::VBO>(data.bo_usage_hint)),
      ebo_(BO<BufferType::EBO>(data.bo_usage_hint)),
      // Check if indices are present to determine draw method
      should_draw_elements_(!data.indices.empty()),
      // Set count based on EBO or VBO size
      count_(static_cast<GLsizei>(data.indices.empty() ? data.vertices.size() / 3 : data.indices.size())),
      draw_mode_(data.draw_mode), // Initialize the draw mode
      color_(data.color),
      model(data.model),
      shader(shader_ptr) {

    vao_.bind();
    vbo_.set_data(data.vertices);

    if (should_draw_elements_) {
        ebo_.set_data(data.indices);
    }

    bool has_texture = (data.flags & Flags::PosTexture) == Flags::PosTexture;
    bool has_normals = (data.flags & Flags::PosNormals) == Flags::PosNormals;

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
    if (has_texture && data.texture_path && !data.texture_path->empty()) {
        texture_ = create_opengl_texture(*data.texture_path);
        if (texture_ == 0) std::cerr << "[Renderer] Failed to create texture\n";
    } else {
        texture_ = UNDEFINED_TEXTURE_LOCATION;
    }
    VAO::unbind();
}

// NEW: Function to dynamically update VBO data (essential for trails)
void Renderer::update_mesh(const MeshData& data) {
    vao_.bind();

    // 1. Update VBO data. This assumes the VBO was initialized with GL_DYNAMIC_DRAW.
    vbo_.set_data(data.vertices);

    // 2. Update state for drawing
    draw_mode_ = data.draw_mode;
    model = data.model;
    color_ = data.color;

    // 3. Update count and draw type (trails typically don't use indices)
    should_draw_elements_ = !data.indices.empty();
    if (should_draw_elements_) {
        // Only update EBO if necessary
        ebo_.set_data(data.indices);
        count_ = static_cast<GLsizei>(data.indices.size());
    } else {
        // For glDrawArrays (LineStrip/Trail), count is the number of vertices.
        count_ = static_cast<GLsizei>(data.vertices.size() / 3);
    }

    // 4. Re-establish vertex attribute pointers for trails (position only: 3 floats)
    int stride = 3 * sizeof(float);
    vao_.vattribptr(0, 3, GL_FLOAT, stride, 0);

    VAO::unbind();
}


void Renderer::draw(const glm::mat4& mvp, const Camera& camera,
                    std::span<const Light> lights, const Material& material) const {

    shader->use();
    shader->set_mat4("mvp", mvp);
    shader->set_mat4("model", model);
    shader->set_vec3("viewPos", camera.position);
    shader->set_vec3("color", color_);

    lightutils::apply_lights_to_shader(*shader, lights);
    material.apply_to_shader(*shader);

    vao_.bind();
    if (texture_ != 0 && texture_ != UNDEFINED_TEXTURE_LOCATION) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_);
        shader->set_int("baseTexture", 0);
    }

    // FIX: Use the dynamic draw mode
    if (should_draw_elements_) {
        glDrawElements(static_cast<GLenum>(draw_mode_), count_, GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawArrays(static_cast<GLenum>(draw_mode_), 0, count_);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::draw(const glm::mat4& mvp, const Camera& camera,
                    std::span<const Light> lights) const {

    shader->use();
    shader->set_mat4("mvp", mvp);
    shader->set_mat4("model", model);
    shader->set_vec3("viewPos", camera.position);
    shader->set_vec3("color", color_);

    shader->set_int("lightCount", static_cast<int>(lights.size()));
    for (int i = 0; i < lights.size(); ++i) {
        shader->set_vec3(std::format("lights[{}].position", i), lights[i].position);
        shader->set_vec3(std::format("lights[{}].color", i), lights[i].color);
        shader->set_float(std::format("lights[{}].intensity", i), lights[i].intensity);
    }

    vao_.bind();
    if (texture_ != 0 && texture_ != UNDEFINED_TEXTURE_LOCATION) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_);
        shader->set_int("baseTexture", 0);
        shader->set_int("shouldDrawBaseTexture", 1);
    } else {
        shader->set_int("shouldDrawBaseTexture", 0);
    }

    // FIX: Use the dynamic draw mode
    if (should_draw_elements_) {
        glDrawElements(static_cast<GLenum>(draw_mode_), count_, GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawArrays(static_cast<GLenum>(draw_mode_), 0, count_);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}
#include "axis_entity.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

AxisEntity::AxisEntity(float axis_length, float arrow_size)
    : axis_length_(axis_length), arrow_size_(arrow_size),
      shader_("./src/shaders/color.vert", "./src/shaders/color.frag")
{
    setup_buffers();
}

AxisEntity::~AxisEntity() {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
}

void AxisEntity::setup_buffers() {
    // Each axis: line + 2 arrow "wings"
    std::vector<glm::vec3> vertices = {
        // X axis
        {0.0f, 0.0f, 0.0f}, {axis_length_, 0.0f, 0.0f},
        {axis_length_, 0.0f, 0.0f}, {axis_length_ - arrow_size_,  arrow_size_, 0.0f},
        {axis_length_, 0.0f, 0.0f}, {axis_length_ - arrow_size_, -arrow_size_, 0.0f},

        // Y axis
        {0.0f, 0.0f, 0.0f}, {0.0f, axis_length_, 0.0f},
        {0.0f, axis_length_, 0.0f}, { arrow_size_, axis_length_ - arrow_size_, 0.0f},
        {0.0f, axis_length_, 0.0f}, {-arrow_size_, axis_length_ - arrow_size_, 0.0f},

        // Z axis
        {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, axis_length_},
        {0.0f, 0.0f, axis_length_}, { 0.0f,  arrow_size_, axis_length_ - arrow_size_},
        {0.0f, 0.0f, axis_length_}, { 0.0f, -arrow_size_, axis_length_ - arrow_size_}
    };

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    // One attribute: position only
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindVertexArray(0);
}

void AxisEntity::update(double) {
    // nothing dynamic
}

void AxisEntity::draw(const Camera& camera, const RenderSettings&, const std::span<const Light> &lights) {
    shader_.use();

    shader_.set_mat4("mvp", camera.mvp);

    glBindVertexArray(vao_);

    // Draw X axis in red
    shader_.set_vec3("axis_color", glm::vec3(1.0f, 0.0f, 0.0f));
    glDrawArrays(GL_LINES, 0, 6);

    // Draw Y axis in green
    shader_.set_vec3("axis_color", glm::vec3(0.0f, 1.0f, 0.0f));
    glDrawArrays(GL_LINES, 6, 6);

    // Draw Z axis in blue
    shader_.set_vec3("axis_color", glm::vec3(0.0f, 0.0f, 1.0f));
    glDrawArrays(GL_LINES, 12, 6);

    glBindVertexArray(0);
}

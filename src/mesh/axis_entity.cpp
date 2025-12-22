#include "axis_entity.h"
#include <../../include/glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <vector>

AxisEntity::AxisEntity(float axis_length, float arrow_size)
    : shader("./src/shaders/color.vert", "./src/shaders/color.frag"), axis_length(axis_length),
    arrow_size(arrow_size) {
    setup_buffers();
}

AxisEntity::~AxisEntity() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void AxisEntity::setup_buffers() {
    // Each axis: line + 2 arrow "wings"
    std::vector<glm::vec3> vertices = {
        // X axis
        {0.0f, 0.0f, 0.0f}, {axis_length, 0.0f, 0.0f},
        {axis_length, 0.0f, 0.0f}, {axis_length - arrow_size,  arrow_size, 0.0f},
        {axis_length, 0.0f, 0.0f}, {axis_length - arrow_size, -arrow_size, 0.0f},

        // Y axis
        {0.0f, 0.0f, 0.0f}, {0.0f, axis_length, 0.0f},
        {0.0f, axis_length, 0.0f}, { arrow_size, axis_length - arrow_size, 0.0f},
        {0.0f, axis_length, 0.0f}, {-arrow_size, axis_length - arrow_size, 0.0f},

        // Z axis
        {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, axis_length},
        {0.0f, 0.0f, axis_length}, { 0.0f,  arrow_size, axis_length - arrow_size},
        {0.0f, 0.0f, axis_length}, { 0.0f, -arrow_size, axis_length - arrow_size}
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    // One attribute: position only
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

    glBindVertexArray(0);
}

void AxisEntity::update(double) {
    // nothing dynamic
}

void AxisEntity::draw(const Camera& camera, const RenderSettings&) {
    shader.use();

    shader.set_mat4("mvp", camera.vp);

    glBindVertexArray(vao);

    // Draw X axis in red
    shader.set_vec3("axis_color", glm::vec3(1.0f, 0.0f, 0.0f));
    glDrawArrays(GL_LINES, 0, 6);

    // Draw Y axis in green
    shader.set_vec3("axis_color", glm::vec3(0.0f, 1.0f, 0.0f));
    glDrawArrays(GL_LINES, 6, 6);

    // Draw Z axis in blue
    shader.set_vec3("axis_color", glm::vec3(0.0f, 0.0f, 1.0f));
    glDrawArrays(GL_LINES, 12, 6);

    glBindVertexArray(0);
}

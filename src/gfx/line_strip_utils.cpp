#include "line_strip_utils.h"

#include <iostream>

#include "bo.h"
#include "math/math_tools.h"
#include "shader.h"
#include "vao.h"

static constexpr int POSITION_COMPONENTS = 3;

LineStripData setup_line_strip(const std::vector<GLfloat>& vertices, GLenum bo_usage_hint) {
    if (!vertices.empty() && vertices.size() % POSITION_COMPONENTS != 0) {
        std::cerr << "[LineStripUtils] Error: Invalid vertex data size for line strip setup.\n";
        return {};
    }

    LineStripData data;

    data.vbo = BO<BufferType::VBO>(bo_usage_hint);
    data.vbo.set_data(vertices);

    data.vao.bind();
    data.vbo.bind();

    int stride_bytes = POSITION_COMPONENTS * sizeof(float);
    data.vao.vattribptr(0, POSITION_COMPONENTS, GL_FLOAT, stride_bytes, 0);

    data.vertex_count = vertices.size() / POSITION_COMPONENTS;

    VAO::unbind();

    return data;
}

LineStripData setup_line_strip(const std::vector<glm::vec3>& vertices, GLenum bo_usage_hint) {
    return setup_line_strip(VectorUtils::flatten_container(vertices), bo_usage_hint);
}

void draw_line_strip(
    const LineStripData& data,
    const std::shared_ptr<Shader>& shader,
    const glm::mat4& mvp,
    const glm::vec3& color
) {
    if (data.vertex_count == 0) {
        return;
    }

    shader->use();
    shader->set_mat4("mvp", mvp);
    shader->set_vec3("color", color);

    data.vao.bind();

    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(data.vertex_count));

    VAO::unbind();
}

void edit_line_strip(LineStripData data, const std::vector<GLfloat> &vertices) {
    data.vbo.set_data(vertices);
    data.vertex_count = vertices.size() / 3;
}

void edit_line_strip(LineStripData data, const std::vector<glm::vec3> &vertices) {
    data.vbo.set_data(vertices);
    data.vertex_count = vertices.size();
}
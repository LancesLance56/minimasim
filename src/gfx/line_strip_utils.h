#ifndef LINE_STRIP_UTILS_H
#define LINE_STRIP_UTILS_H

#include "gfx/bo.h" // For BO<T> and BufferType::VBO
#include "gfx/shader.h" // For Shader
#include "gfx/vao.h" // For VAO

#include <glm/glm.hpp>
#include <memory>
#include <span>
#include <string>

struct LineStripData {
    VAO vao{};
    BO<BufferType::VBO> vbo{};
    size_t vertex_count = 0;
};

LineStripData setup_line_strip(const std::vector<float>& vertices, GLenum bo_usage_hint);
LineStripData setup_line_strip(const std::vector<glm::vec3>& vertices, GLenum bo_usage_hint);

void draw_line_strip(
    const LineStripData& data,
    const std::shared_ptr<Shader>& shader,
    const glm::mat4& mvp,
    const glm::vec3& color = glm::vec3(1.0f)
);

void edit_line_strip(LineStripData data, const std::vector<float> &vertices);
void edit_line_strip(LineStripData data, const std::vector<glm::vec3> &vertices);


#endif // LINE_STRIP_UTILS_H

#ifndef RENDERER_H
#define RENDERER_H

#include "component.h" // Include the new CRTP base
#include "mesh_filter_component.h"
#include "../../../gfx/bo.h"
#include "../../../gfx/light.h"
#include "../../../gfx/material.h"
#include "../../../gfx/shader.h"
#include "../../../gfx/vao.h"

#include <memory>
#include <span>


class MeshRenderer : public Component<MeshRenderer> {
private:
    VAO vao_{};
    BO<BufferType::VBO> vbo_{};
    BO<BufferType::EBO> ebo_{};
    GLuint texture;
    bool should_draw_elements_ = false;

    int vertices_count;
    int indices_count;

    const MeshFilter &mesh_filter;
public:
    glm::mat4 model{1.0f};
    glm::mat4 mvp{1.0f};
    std::shared_ptr<Shader> shader;

    MeshRenderer() = delete;
    MeshRenderer(const std::shared_ptr<Shader>& shader, const MeshFilter& mesh);

    void draw(const glm::mat4& mvp,
              const Camera& camera,
              std::span<const Light> lights,
              const Material& material) const;

    void draw(const glm::mat4& mvp,
              const Camera& camera,
              std::span<const Light> lights) const;
};

#endif // RENDERER_H
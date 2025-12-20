#ifndef RENDERER_H
#define RENDERER_H

#include "../../../gfx/bo.h"
#include "../../../gfx/camera.h"
#include "../../../gfx/light.h"
#include "../../../gfx/material.h"
#include "../../../gfx/shader.h"
#include "../../../gfx/texture.h"
#include "../../../gfx/vao.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <optional>
#include <span>
#include <vector>

class Renderer {
public:
    enum class Flags : uint8_t {
        None        = 0,
        PosNormals  = 1 << 0,
        PosTexture  = 1 << 1,
        PosNormalsTex  = PosNormals | PosTexture
    };

    enum class DrawMode : GLenum {
        Triangles = GL_TRIANGLES,
        LineStrip = GL_LINE_STRIP,
        Points    = GL_POINTS,
        Lines     = GL_LINES
    };

private:
    VAO vao_{};
    BO<BufferType::VBO> vbo_{};
    BO<BufferType::EBO> ebo_{};
    bool should_draw_elements_ = false;

    static constexpr GLuint UNDEFINED_TEXTURE_LOCATION = 180;
    GLuint texture_ = UNDEFINED_TEXTURE_LOCATION;
    GLsizei count_ = 0;

    // FIX 2: Store the current draw mode
    DrawMode draw_mode_ = DrawMode::Triangles;

    glm::vec3 color_ = {1.0f, 1.0f, 1.0f};

public:
    glm::mat4 model{1.0f};
    glm::mat4 mvp{1.0f};
    std::shared_ptr<Shader> shader;

    // Settings struct for mesh creation
    struct MeshData {
        std::vector<GLfloat> vertices;
        std::vector<GLuint> indices;
        glm::vec3 color{1.0f, 1.0f, 1.0f};
        std::optional<std::string> texture_path;
        Flags flags = Flags::None;
        glm::mat4 model{1.0f};
        GLuint bo_usage_hint = GL_STATIC_DRAW;
        // FIX 3: Add DrawMode to MeshData
        DrawMode draw_mode = DrawMode::Triangles;
    };

    Renderer() = default;
    Renderer(const std::shared_ptr<Shader>& shader, const MeshData& data);

    // FIX 4: Add the mesh update function for dynamic geometry like trails
    void update_mesh(const MeshData& data);

    void draw(const glm::mat4& mvp,
              const Camera& camera,
              std::span<const Light> lights,
              const Material& material) const;

    void draw(const glm::mat4& mvp,
              const Camera& camera,
              std::span<const Light> lights) const;
};

// Bitwise operators for Flags
inline Renderer::Flags operator|(Renderer::Flags a, Renderer::Flags b) {
    return static_cast<Renderer::Flags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}
inline Renderer::Flags operator&(Renderer::Flags a, Renderer::Flags b) {
    return static_cast<Renderer::Flags>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

#endif
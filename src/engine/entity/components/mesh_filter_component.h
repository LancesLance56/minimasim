#ifndef MESH_FILTER_COMPONENT_H // Renamed guard
#define MESH_FILTER_COMPONENT_H

#include "component.h" // Include the new CRTP base
#include "shader.h"
#include "material.h"

#include <optional>
#include <string>
#include <vector>

#include "gfx.h"

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

struct MeshData {
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    std::optional<std::string> texture_path;
    Flags flags = Flags::None;
    Material material;
    glm::mat4 model{1.0f};
    GLuint bo_usage_hint = GL_STATIC_DRAW;
    DrawMode draw_mode = DrawMode::Triangles;
};

class MeshFilter : public Component<MeshFilter> {
public:
    MeshData mesh_data;

    MeshFilter() = default;

    /**
     * @param vertices Interleaved vertex data
     * @param indices Triangle indices
     * @param texture_path Optional texture path
     * @param material Material
     * @param has_normals Whether vertices contain normals
     * @param model Model matrix
     */
    MeshFilter(const std::vector<GLfloat>& vertices,
              const std::vector<GLuint>& indices,
              const std::optional<std::string>& texture_path = std::nullopt,
              const Material& material = blue_base_material,
              bool has_normals = false,
              const glm::mat4& model = glm::mat4(1.0f));

};

#endif // MESH_FILTER_COMPONENT_H

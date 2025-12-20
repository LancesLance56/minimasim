#ifndef MESH_COMPONENT_H // Renamed guard
#define MESH_COMPONENT_H

#include "component.h" // Include the new CRTP base
#include "mesh_renderer_component.h"
#include "shader.h"

#include <optional>
#include <string>
#include <vector>

// Renamed class from Mesh to MeshComponent and added CRTP inheritance
class MeshComponent : public Component<MeshComponent> {
public:
    MeshRenderer renderer;
    std::vector<GLuint> indices;
    std::vector<GLfloat> vertices;
    glm::vec3 color{1.0f, 1.0f, 1.0f};
    GLuint texture{}; // optional, can be UNDEFINED_TEXTURE_LOCATION

    MeshComponent() = default;

    /**
     * @param vertices Interleaved vertex data
     * @param indices Triangle indices
     * @param shader Shader to use
     * @param color Default color (used if no texture)
     * @param texture_path Optional texture path
     * @param has_normals Whether vertices contain normals
     * @param model Model matrix
     */
    MeshComponent(const std::vector<GLfloat>& vertices,
                  const std::vector<GLuint>& indices,
                  const std::shared_ptr<Shader>& shader,
                  glm::vec3 color = glm::vec3(1.0f),
                  const std::optional<std::string>& texture_path = std::nullopt,
                  bool has_normals = false,
                  const glm::mat4& model = glm::mat4(1.0f));
};

#endif // MESH_COMPONENT_H

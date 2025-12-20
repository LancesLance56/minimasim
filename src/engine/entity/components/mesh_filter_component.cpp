#include "mesh_component.h"

// Updated constructor name from Mesh::Mesh to MeshComponent::MeshComponent
MeshComponent::MeshComponent(const std::vector<GLfloat>& vertices,
           const std::vector<GLuint>& indices,
           const std::shared_ptr<Shader>& shader,
           glm::vec3 color,
           const std::optional<std::string>& texture_path,
           bool has_normals,
           const glm::mat4& model)
    : indices(indices), vertices(vertices), color(color) {
    using enum MeshRenderer::Flags;
    // Determine MeshRenderer flags
    auto flags = None;
    if (has_normals && texture_path) flags = PosNormalsTex;
    else if (has_normals) flags = PosNormals;
    else if (texture_path) flags = PosTexture;

    // Prepare MeshRenderer mesh data
    MeshRenderer::MeshData data;
    data.vertices = vertices;
    data.indices = indices;
    data.color = color;
    data.texture_path = texture_path;
    data.flags = flags;
    data.model = model;

    // Construct the MeshRenderer
    renderer = MeshRenderer(shader, data);
}

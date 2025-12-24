#include "mesh_filter_component.h"

#include <iostream>
#include <ostream>

// Updated constructor name from Mesh::Mesh to MeshFilter::MeshFilter
MeshFilter::MeshFilter(
        const std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices,
        const std::optional<std::string> &texture_path, const Material &material, bool has_normals,
        const glm::mat4 &model) {
    using enum Flags;
    // Determine MeshRenderer flags
    auto flags = None;
    if (has_normals && texture_path)
        flags = PosNormalsTex;
    else if (has_normals)
        flags = PosNormals;
    else if (texture_path)
        flags = PosTexture;

    // Prepare MeshRenderer mesh data
    mesh_data.vertices = vertices;
    mesh_data.indices = indices;
    mesh_data.material = material;
    mesh_data.texture_path = texture_path;
    mesh_data.flags = flags;
    mesh_data.model = model;
}

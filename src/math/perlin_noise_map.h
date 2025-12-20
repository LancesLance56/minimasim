#ifndef PERLIN_MESH_H
#define PERLIN_MESH_H

#include "glad_include.h"
#include <vector>

class PerlinNoiseMap {
public:
    int square_length;
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    explicit PerlinNoiseMap(const struct PerlinPlaneSettings &settings);
    void recalculate_vertex_normals(std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices, const struct PerlinPlaneSettings& settings) const;
};


#endif // PERLIN_MESH_H

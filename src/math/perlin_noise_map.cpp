#include "../math/perlin_noise_map.h"

#include <algorithm>

#include "perlin.h"

#include <cmath>
#include <cstdlib>
#include <glm/glm.hpp>
#include <iostream>
#include <random>
#include <vector>

// position from vertex: pos, tex, norm
glm::vec3 getVertexPos(const float *vertices, const int index, const int stride) {
    return {vertices[index* stride + 0], vertices[index* stride + 1], vertices[index* stride + 2]};
}

PerlinNoiseMap::PerlinNoiseMap(const PerlinPlaneSettings &settings) :
    square_length(settings.sizeOfMesh) {
    int N = this->square_length* settings.meshResolution;
    int floatsPerVertex = 8;

    int vertexCount = N* N;
    int indexCount = (N - 1) * (N - 1) * 6;

    vertices.resize(vertexCount* floatsPerVertex);
    indices.resize(indexCount);

    // Initialize vertices: pos (3) + tex (2) + norm (3) = 8 floats
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int vi = i* N + j;
            int offset = vi* floatsPerVertex;

            float x = static_cast<float>(i) / static_cast<float>(settings.meshResolution);
            float z = static_cast<float>(j) / static_cast<float>(settings.meshResolution);

            float raw_y = perlinNoise(
                                  (static_cast<float>(i) + settings.x_offset) / static_cast<float>(settings.meshResolution),
                                  (static_cast<float>(j) + settings.z_offset) / static_cast<float>(settings.meshResolution), settings.seed, settings.scale,
                                  settings.octaves, settings.persistence, settings.lacunarity);

            if (!std::isfinite(raw_y)) {
                std::cerr << "Invalid height detected at (" << i << ", " << j << "): " << raw_y << std::endl;
                raw_y = 0.0f;
            }

            float y = raw_y* settings.heightMultiplier;

            vertices[offset + 0] = x;
            vertices[offset + 1] = y;
            vertices[offset + 2] = z;

            vertices[offset + 3] = static_cast<float>(j) * 10 / static_cast<float>(N - 1); // tex u
            vertices[offset + 4] = static_cast<float>(i) * 10 / static_cast<float>(N - 1); // tex v

            vertices[offset + 5] = 0.0f; // normal x
            vertices[offset + 6] = 0.0f; // normal y
            vertices[offset + 7] = 0.0f; // normal z
        }
    }

    // Build index buffer (2 triangles per quad)
    int idx = 0;
    for (int i = 0; i < N - 1; ++i) {
        for (int j = 0; j < N - 1; ++j) {
            int a = j + N* i;
            int b = j + N* (i + 1);
            int c = (j + 1) + N* i;
            int d = (j + 1) + N* (i + 1);

            // Triangle 1: a, b, c
            indices[idx] = a; ++idx;
            indices[idx] = b; ++idx;
            indices[idx] = d; ++idx;

            // Triangle 2: c, b, d
            indices[idx] = a; ++idx;
            indices[idx] = d; ++idx;
            indices[idx] = c; ++idx;
        }
    }

    // Calculate normals
    for (int i = 0; i < indexCount; i += 3) {
        unsigned int i0 = indices[i];
        unsigned int i1 = indices[i + 1];
        unsigned int i2 = indices[i + 2];

        glm::vec3 v0 = getVertexPos(vertices.data(), i0, floatsPerVertex);
        glm::vec3 v1 = getVertexPos(vertices.data(), i1, floatsPerVertex);
        glm::vec3 v2 = getVertexPos(vertices.data(), i2, floatsPerVertex);

        glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

        for (int vi: {i0, i1, i2}) {
            vertices[vi* floatsPerVertex + 5] += normal.x;
            vertices[vi* floatsPerVertex + 6] += normal.y;
            vertices[vi* floatsPerVertex + 7] += normal.z;
        }
    }

    // Normalize vertex normals
    for (int i = 0; i < vertexCount; ++i) {
        glm::vec3 n(
                vertices[i* floatsPerVertex + 5], vertices[i* floatsPerVertex + 6],
                vertices[i* floatsPerVertex + 7]);
        if (glm::length(n) > 0.0001f && std::isfinite(glm::length(n)))
            n = glm::normalize(n);
        else
            n = glm::vec3(0.0f, 1.0f, 0.0f); // default up

        n = glm::normalize(n);
        vertices[i* floatsPerVertex + 5] = n.x;
        vertices[i* floatsPerVertex + 6] = n.y;
        vertices[i* floatsPerVertex + 7] = n.z;
    }
}

void PerlinNoiseMap::recalculate_vertex_normals(std::vector<GLfloat> &vertices, const std::vector<GLuint> &indices, const struct PerlinPlaneSettings& settings) const {
    int floatsPerVertex = 8;

    int N = square_length* settings.meshResolution;
    int vertexCount = N* N;
    // Normalize vertex normals
    for (int i = 0; i < vertexCount; ++i) {
        glm::vec3 n(
                vertices[i* floatsPerVertex + 5], vertices[i* floatsPerVertex + 6],
                vertices[i* floatsPerVertex + 7]);
        if (glm::length(n) > 0.0001f && std::isfinite(glm::length(n)))
            n = glm::normalize(n);
        else
            n = glm::vec3(0.0f, 1.0f, 0.0f); // default up

        n = glm::normalize(n);
        vertices[i* floatsPerVertex + 5] = n.x;
        vertices[i* floatsPerVertex + 6] = n.y;
        vertices[i* floatsPerVertex + 7] = n.z;
    }
}

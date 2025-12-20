#include "cube_entity.h"

#include "engine/precompiled_shaders.h"

std::vector<GLfloat> get_colored_cube_vertices() {
    return {
        -0.5f, -0.5f, 0.5f, // 0
        0.5f, -0.5f, 0.5f, // 1
        0.5f, 0.5f, 0.5f, // 2
        -0.5f, 0.5f, 0.5f, // 3

        -0.5f, -0.5f, -0.5f, // 4
        0.5f, -0.5f, -0.5f, // 5
        0.5f, 0.5f, -0.5f, // 6
        -0.5f, 0.5f, -0.5f // 7{
    };
}

std::vector<GLfloat> get_phong_cube_vertices() {
    return {
        // Front face (0,0,1)
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,

        // Back face (0,0,-1)
        -0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   0.0f, 0.0f,-1.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f, 0.0f,-1.0f,
         0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   0.0f, 0.0f,-1.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   0.0f, 0.0f,-1.0f,

        // Left face (-1,0,0)
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,

        // Right face (1,0,0)
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   1.0f, 0.0f, 0.0f,

        // Top face (0,1,0)
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f, 1.0f, 0.0f,

        // Bottom face (0,-1,0)
        -0.5f, -0.5f, -0.5f,   1.0f, 1.0f,   0.0f,-1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   0.0f,-1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f,-1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f,-1.0f, 0.0f
    };
}


std::vector<GLuint> get_colored_cube_indices() {
    return {
        0, 2, 1, 0, 3, 2,
        1, 6, 5, 1, 2, 6,
        5, 7, 4, 5, 6, 7,
        4, 3, 0, 4, 7, 3,
        4, 1, 5, 4, 0, 1,
        3, 6, 2, 3, 7, 6
    };
}

std::vector<GLuint> get_phong_cube_indices() {
    return {
        0, 2, 1, 0, 3, 2,       // Front
        4, 6, 5, 4, 7, 6,       // Back
        8, 10,9, 8,11,10,       // Left
       12,14,13,12,15,14,       // Right
       16,18,17,16,19,18,       // Top
       20,22,21,20,23,22        // Bottom
    };
}


CubeEntity::CubeEntity(const glm::vec3 position, const glm::vec3 rotation, const float size, const glm::vec3 rgb,
        const std::string &texture_path) {
    this->position = position;
    this->rotation = rotation;
    this->size = size;
    this->color_rgb = rgb;
    this->texture_path = texture_path;

    colored_cube_mesh = Mesh(MeshType::COLOR, PrecompiledShaders::colored_shader, "", get_colored_cube_indices(), get_colored_cube_vertices(), 36, 24, rgb);
    phong_cube_mesh = Mesh(MeshType::COLOR_TEXTURE_NORMALS, PrecompiledShaders::phong_shader, texture_path, get_phong_cube_indices(), get_phong_cube_vertices(), 36, 192, rgb);
}

void CubeEntity::draw(
        const Camera &camera, const RenderSettings &render_settings, const std::span<const Light> &light_entities) {
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(size, size, size));

    phong_cube_mesh.rgb = color_rgb;
    phong_cube_mesh.renderer.draw(camera.mvp * model, camera, light_entities);
}

void CubeEntity::draw(
        const Camera &camera, const RenderSettings &render_settings) {
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(size, size, size));

    colored_cube_mesh.rgb = color_rgb;
    colored_cube_mesh.renderer.draw(camera.mvp * model, camera, {});
}

void CubeEntity::update(double dt) { }
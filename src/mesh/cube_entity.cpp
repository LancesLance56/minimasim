#include "cube_entity.h"
#include "engine/entity/components/mesh_renderer_component.h"
#include "engine/precompiled_shaders.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

std::vector<GLfloat> get_colored_cube_vertices() {
    return {
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f
    };
}

std::vector<GLuint> get_colored_cube_indices() {
    return {
        0,2,1, 0,3,2, // Front
        1,6,5, 1,2,6, // Right
        5,7,4, 5,6,7, // Back
        4,3,0, 4,7,3, // Left
        4,1,5, 4,0,1, // Bottom
        3,6,2, 3,7,6  // Top
    };
}

std::vector<GLfloat> get_phong_cube_vertices() {
    return {
        // position     texcoord   normal
        // Front face
        -0.5f,-0.5f, 0.5f, 0,0, 0,0,1,
         0.5f,-0.5f, 0.5f, 1,0, 0,0,1,
         0.5f, 0.5f, 0.5f, 1,1, 0,0,1,
        -0.5f, 0.5f, 0.5f, 0,1, 0,0,1,
        // Back face
        -0.5f,-0.5f,-0.5f, 1,0, 0,0,-1,
        -0.5f, 0.5f,-0.5f, 1,1, 0,0,-1,
         0.5f, 0.5f,-0.5f, 0,1, 0,0,-1,
         0.5f,-0.5f,-0.5f, 0,0, 0,0,-1,
        // Left face
        -0.5f,-0.5f,-0.5f, 0,0, -1,0,0,
        -0.5f,-0.5f, 0.5f, 1,0, -1,0,0,
        -0.5f, 0.5f, 0.5f, 1,1, -1,0,0,
        -0.5f, 0.5f,-0.5f, 0,1, -1,0,0,
        // Right face
         0.5f,-0.5f,-0.5f, 1,0, 1,0,0,
         0.5f, 0.5f,-0.5f, 1,1, 1,0,0,
         0.5f, 0.5f, 0.5f, 0,1, 1,0,0,
         0.5f,-0.5f, 0.5f, 0,0, 1,0,0,
        // Top face
        -0.5f, 0.5f,-0.5f, 0,1, 0,1,0,
        -0.5f, 0.5f, 0.5f, 0,0, 0,1,0,
         0.5f, 0.5f, 0.5f, 1,0, 0,1,0,
         0.5f, 0.5f,-0.5f, 1,1, 0,1,0,
        // Bottom face
        -0.5f,-0.5f,-0.5f, 1,1, 0,-1,0,
         0.5f,-0.5f,-0.5f, 0,1, 0,-1,0,
         0.5f,-0.5f, 0.5f, 0,0, 0,-1,0,
        -0.5f,-0.5f, 0.5f, 1,0, 0,-1,0
    };
}

std::vector<GLuint> get_phong_cube_indices() {
    return {
        0,2,1, 0,3,2,       // Front
        4,6,5, 4,7,6,       // Back
        8,10,9, 8,11,10,    // Left
        12,14,13, 12,15,14, // Right
        16,18,17, 16,19,18, // Top
        20,22,21, 20,23,22  // Bottom
    };
}

ColoredCubeEntity::ColoredCubeEntity(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 rgb)
    : BaseCubeEntity(position, rotation, scale, rgb) {
    this->add_component<MeshFilter>(get_colored_cube_vertices(), get_colored_cube_indices(), std::nullopt, Material(rgb, 32));
    this->add_component<MeshRenderer>(precompiled_shaders::colored_shader);
}

void ColoredCubeEntity::draw(const Camera &camera, const RenderSettings &render_settings) {
    glm::mat4 model(1.0f);
    model = glm::translate(model, transform.position);
    model = glm::rotate(model, transform.rotation.x, glm::vec3(1,0,0));
    model = glm::rotate(model, transform.rotation.y, glm::vec3(0,1,0));
    model = glm::rotate(model, transform.rotation.z, glm::vec3(0,0,1));
    model = glm::scale(model, transform.scale);

    auto mesh_renderer = std::move(this->get_component<MeshRenderer>());
    mesh_renderer.model = model;
    mesh_renderer.draw(camera.vp * model, camera, {}, blue_base_material);
}

PhongCubeEntity::PhongCubeEntity(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 rgb,
                                 const std::string &texture_path)
    : BaseCubeEntity(position, rotation, scale, rgb) {
    this->add_component<MeshFilter>(get_phong_cube_vertices(), get_phong_cube_indices(), std::nullopt, Material(rgb, 32));
    this->add_component<MeshRenderer>(precompiled_shaders::phong_normals_shader);
}

void PhongCubeEntity::draw(const Camera &camera, const RenderSettings &render_settings,
                           const std::span<const Light> &light_entities) {
    glm::mat4 model(1.0f);
    model = glm::translate(model, transform.position);
    model = glm::rotate(model, transform.rotation.x, glm::vec3(1,0,0));
    model = glm::rotate(model, transform.rotation.y, glm::vec3(0,1,0));
    model = glm::rotate(model, transform.rotation.z, glm::vec3(0,0,1));
    model = glm::scale(model, transform.scale);

    auto mesh_renderer = std::move(this->get_component<MeshRenderer>());
    mesh_renderer.model = model;
    mesh_renderer.draw(camera.vp* model, camera, light_entities, material);
}

void PhongCubeEntity::on_gui(int entity_id) {
    ImGui::Begin("Phong Cube Entity");

    ImGui::Text("Transform");
    ImGui::SliderFloat3("Position", glm::value_ptr(transform.position), -1.0f, 1.0f);
    ImGui::SliderFloat3("Euler Angles", glm::value_ptr(transform.rotation), -glm::pi<float>(), glm::pi<float>());
    ImGui::SliderFloat3("Scale", glm::value_ptr(transform.scale), -1.0f, 1.0f);

    ImGui::Text("Material: ");
    material.display_dear_imgui();

    ImGui::End();
}

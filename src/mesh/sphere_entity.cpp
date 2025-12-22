#include "sphere_entity.h"
#include "sphere_entity_constants.h"

#include <../../include/glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <format>
#include <map>

#include "engine/precompiled_shaders.h"
#include "imgui.h"

MeshData generate_icosphere(int, bool);

SphereEntity::SphereEntity(glm::vec3 position, int subdivisions, float r, bool has_normals, const Material &material)
    : material(material), radius(r) {
    transform.position = position;
    transform.rotation = glm::vec3(0.0f);
    transform.scale = glm::vec3(r);

    MeshData mesh_data = generate_icosphere(subdivisions, has_normals);
    this->add_component<MeshFilter>(mesh_data.vertices, mesh_data.indices, std::nullopt, material, has_normals);
    this->add_component<MeshRenderer>(precompiled_shaders::phong_normals_shader);
}

MeshData generate_icosphere(int subdivisions, bool has_normals) {
    std::vector<float> vertex_data;
    std::vector<GLuint> indices;

    if (has_normals && subdivisions == 2) {
        vertex_data = two_subdivisions_with_normals_vertices;
        indices = two_subdivisions_indices;
    } else if (has_normals && subdivisions == 3) {
        vertex_data = three_subdivisions_with_normals_vertices;
        indices = three_subdivisions_indices;
    } else {
        constexpr auto t = glm::golden_ratio<float>();
        std::vector<glm::vec3> positions = {
            {-1,  t,  0}, { 1,  t,  0}, {-1, -t,  0}, { 1, -t,  0},
            { 0, -1,  t}, { 0,  1,  t}, { 0, -1, -t}, { 0,  1, -t},
            { t,  0, -1}, { t,  0,  1}, {-t,  0, -1}, {-t,  0,  1}
        };
        for (auto &p : positions) p = glm::normalize(p);

        std::vector<GLuint> init_indices = {
            0,5,11,  0,1,5,   0,7,1,   0,10,7,  0,11,10,
            1,9,5,   5,4,11,  11,2,10, 10,6,7,  7,8,1,
            3,4,9,   3,2,4,   3,6,2,   3,8,6,   3,9,8,
            4,5,9,   2,11,4,  6,10,2,  8,7,6,   9,1,8
        };
        indices.assign(std::begin(init_indices), std::end(init_indices));

        std::map<std::pair<GLuint, GLuint>, GLuint> mid_cache;
        auto get_mid_point = [&](const GLuint a, const GLuint b) {
            const auto idx = static_cast<GLuint>(positions.size());
            const auto key = std::minmax(a, b);

            if (mid_cache.contains(key))
                return mid_cache[key];

            positions.push_back(glm::normalize((positions[a] + positions[b]) * 0.5f));
            mid_cache[key] = idx;

            return idx;
        };

        for (int i = 0; i < subdivisions; ++i) {
            std::vector<GLuint> new_indices;
            new_indices.reserve(indices.size() * 4);
            for (size_t j = 0; j < indices.size(); j += 3) {
                GLuint a = indices[j];
                GLuint b = indices[j+1];
                GLuint c = indices[j+2];

                GLuint ab = get_mid_point(a, b);
                GLuint bc = get_mid_point(b, c);
                GLuint ca = get_mid_point(c, a);

                new_indices.insert(new_indices.end(), {a, ca, ab});
                new_indices.insert(new_indices.end(), {b, ab, bc});
                new_indices.insert(new_indices.end(), {c, bc, ca});
                new_indices.insert(new_indices.end(), {ab, ca, bc});
            }
            indices = std::move(new_indices);
        }

        vertex_data.reserve(positions.size() * (has_normals ? 6 : 3));

        for (auto const &p : positions) {
            glm::vec3 normal = glm::normalize(p);

            vertex_data.push_back(p.x);
            vertex_data.push_back(p.y);
            vertex_data.push_back(p.z);

            if (has_normals) {
                vertex_data.push_back(normal.x);
                vertex_data.push_back(normal.y);
                vertex_data.push_back(normal.z);
            }
        }
    }

    MeshData data;
    data.vertices = vertex_data;
    data.indices = indices;
    data.model = glm::mat4(1.0f);
    data.flags = has_normals ? Flags::PosNormals : Flags::None;

    return data;
}

glm::mat4 SphereEntity::compute_model_matrix() const {
    glm::mat4 model(1.0f);
    model = glm::translate(model, transform.position);
    model = glm::rotate(model, transform.rotation.x, glm::vec3(1,0,0));
    model = glm::rotate(model, transform.rotation.y, glm::vec3(0,1,0));
    model = glm::rotate(model, transform.rotation.z, glm::vec3(0,0,1));
    model = glm::scale(model, glm::vec3(radius));
    model = glm::scale(model, transform.scale);

    return model;
}

void SphereEntity::draw(const Camera &cam, const RenderSettings &rs) {
    auto model = compute_model_matrix();
    this->get_component<MeshRenderer>().model = model;
    this->get_component<MeshRenderer>().draw(cam.vp, cam, {}, material);
}

void SphereEntity::draw(const Camera &cam, const RenderSettings &rs, const std::span<const Light> &lights) {
    auto model = compute_model_matrix();
    this->get_component<MeshRenderer>().model = model;
    this->get_component<MeshRenderer>().draw(cam.vp, cam, lights, material);
}

void SphereEntity::on_gui(int entity_id) {
    const auto title = std::format("Sphere Entity##{}", entity_id);
    ImGui::Begin(title.c_str());

    ImGui::Text("Material");
    material.display_dear_imgui();

    ImGui::Separator();
    ImGui::Text("Transform");
    ImGui::DragFloat3("Position", &transform.position.x, 0.1f);
    ImGui::DragFloat3("Rotation (rad)", &transform.rotation.x, 0.01f);
    ImGui::DragFloat3("Scale", &transform.scale.x, 0.05f, 0.01f, 100.0f);
    ImGui::Text(std::format("Entity ID: {}", entity_id).c_str());

    ImGui::End();
}
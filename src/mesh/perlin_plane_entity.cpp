#include "perlin_plane_entity.h"

PerlinPlaneEntity::PerlinPlaneEntity(const std::shared_ptr<Window> &window, const glm::vec3 &position) :
    previous_perlin_plane_settings_(perlin_plane_settings), last_curve_(curve_) {
    perlin_plane_shader_ = std::make_shared<Shader>("./src/shaders/textured_terrain.vert", "./src/shaders/textured_terrain.frag");
    height_map_lut_ = BezierEditor::computeHeightRemapLUT(curve_);
    perlin_plane_ = PerlinPlane(perlin_plane_shader_, perlin_plane_settings, height_map_lut_, position);
    window_ = window;
    editor = BezierEditor(window->window);
}

void PerlinPlaneEntity::draw(const Camera &camera, const RenderSettings &render_settings, const std::span<const Light> &light_entities) {
    perlin_plane_.draw(camera, render_settings, light_entities);
}

void PerlinPlaneEntity::update(double dt) {
    if (perlin_plane_settings != previous_perlin_plane_settings_ || last_curve_ != curve_) {
        if (last_curve_ != curve_ && glfwGetMouseButton(window_->window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS ) {
            height_map_lut_ = BezierEditor::computeHeightRemapLUT(curve_);
        }
        perlin_plane_ = PerlinPlane(perlin_plane_shader_, perlin_plane_settings, height_map_lut_, perlin_plane_.position_);
        previous_perlin_plane_settings_ = perlin_plane_settings;
        last_curve_ = curve_;
    }
}

void PerlinPlaneEntity::on_gui(int entity_id) {
    ImGui::Begin("Perlin plane entity");
    ImGui::SliderFloat2("Offset Position", &perlin_plane_settings.x_offset, -100.0f, 100.0f);
    ImGui::InputInt("Seed", &perlin_plane_settings.seed);

    if (auto res = static_cast<int>(perlin_plane_settings.meshResolution);
        ImGui::InputInt("Resolution", &res))
        perlin_plane_settings.meshResolution = std::max(1, res);

    ImGui::SliderFloat("Scale", &perlin_plane_settings.scale, 0.0f, 30.0f);
    ImGui::SliderFloat("Height Multiplier", &perlin_plane_settings.heightMultiplier, 0.0f, 30.0f);
    ImGui::InputInt("Octaves", &perlin_plane_settings.octaves);
    ImGui::SliderFloat("Persistence", &perlin_plane_settings.persistence, 0.0f, 3.0f);
    ImGui::SliderFloat("Lacunarity", &perlin_plane_settings.lacunarity, 0.0f, 3.0f);
    ImGui::SliderFloat3("Position", &perlin_plane_.position_.x, -30.0f, 30.0f);

    if (ImGui::InputInt("Mesh Size", std::bit_cast<int*>(&perlin_plane_settings.sizeOfMesh))) {
        perlin_plane_settings.sizeOfMesh = std::max(1u, perlin_plane_settings.sizeOfMesh);
    }
    ImGui::End();

    ImGui::Begin("Editor");
    editor.Draw("My Bezier Curve", curve_);
    ImGui::End();
}

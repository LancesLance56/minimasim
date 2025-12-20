#ifndef PERLIN_PLANE_ENTITY_H
#define PERLIN_PLANE_ENTITY_H

#include "../engine/entity/entity.h"
#include "../engine/render_settings.h"
#include "imgui/animation_curve.h"
#include "perlin_plane.h"
#include "window.h"

class PerlinPlaneEntity : public Entity<PerlinPlaneEntity> {
public:
    PerlinPlaneSettings perlin_plane_settings = {
        .sizeOfMesh = 100,
        .meshResolution = 1,
        .x_offset = 0.0f,
        .z_offset = 0.0f,
        .scale = 25.0f,
        .seed = 0,
        .heightMultiplier = 5.0f,
        .octaves = 5,
        .persistence = 0.5f,
        .lacunarity = 2.0f
    };

    explicit PerlinPlaneEntity(const std::shared_ptr<Window> &window, const glm::vec3 &position = glm::vec3(0.0f));
    void draw(const Camera &camera, const RenderSettings &render_settings, const std::span<const Light> &light_entities) override;
    void update(double dt) override;
    void on_gui(int entity_id) override;
private:
    BezierEditor editor;
    std::shared_ptr<Window> window_;
    std::shared_ptr<Shader> perlin_plane_shader_;
    PerlinPlane perlin_plane_;
    PerlinPlaneSettings previous_perlin_plane_settings_;
    std::vector<float> height_map_lut_;
    std::vector<BezierKeyframe> curve_ = {
        {0.0f, 0.0f, ImVec2(30, -0)},
        {0.25f, 0.1f, ImVec2(40, -10)},
        {0.5f, 0.2f, ImVec2(30, -20)},
        {0.75f, 0.4f, ImVec2(20, -20)},
        {1.0f, 1.0f, ImVec2(30, -30)}
    };
    std::vector<BezierKeyframe> &last_curve_;
};

#endif //PERLIN_PLANE_ENTITY_H

#include "trail_component.h"

#include <vector> // Ensure vector is included for the conversion

#include "engine/precompiled_shaders.h"
#include "math/math_tools.h"
#include "mesh_renderer_component.h"

TrailComponent3D::TrailComponent3D(int data_point_count, glm::vec3 color)
    : data_point_count(data_point_count) {
    line_strip_data = setup_line_strip(std::vector<glm::vec3>{}, GL_DYNAMIC_DRAW);
}

void TrailComponent3D::append_trail(const glm::vec3 position) {
    positions.push_back(position);

    if (positions.size() > data_point_count) {
        positions.pop_front();
    }

    std::vector temp_vec(positions.begin(), positions.end());

    edit_line_strip(std::move(line_strip_data), temp_vec);
}

void TrailComponent3D::draw(const Camera &camera, const RenderSettings &render_settings, const std::span<const Light> &lights) {
    if (line_strip_data.vertex_count > 1) {
        draw_line_strip(line_strip_data, precompiled_shaders::colored_shader, camera.vp);
    }
}
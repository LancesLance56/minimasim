#ifndef TRAIL_COMPONENT_H
#define TRAIL_COMPONENT_H
#include <deque>
#include <vector>

#include "camera.h"
#include "line_strip_utils.h"
#include "component.h" // Make sure to include the new component.h

#include "../../../../include/glm/detail/type_vec.hpp"

class TrailComponent3D : public Component<TrailComponent3D> {
public:
    explicit TrailComponent3D(int data_point_count = 100, glm::vec3 color = glm::vec3(1.0f));
    void append_trail(glm::vec3 position);

    void draw(const Camera& camera, const RenderSettings& render_settings, const std::span<const Light> &lights);

private:
    std::deque<glm::vec3> positions;
    int data_point_count;

    LineStripData line_strip_data;
};

#endif //TRAIL_COMPONENT_H
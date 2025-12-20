#ifndef LIGHTPOS_H
#define LIGHTPOS_H

#include <vector>
#include <glm/glm.hpp>

struct Light {
    glm::vec3 position;
    glm::vec3 color;
};

extern std::vector<Light> global_lights;
extern std::vector<Light> global_lights_last_frame;

#endif //LIGHTPOS_H

#include "lightpos.h"

std::vector<Light> global_lights = {
    {glm::vec3(4.0f, 5.0f, 7.0f), glm::vec3(4.0f, 4.0f, 4.0f)},
};

std::vector<Light> global_lights_last_frame = global_lights;
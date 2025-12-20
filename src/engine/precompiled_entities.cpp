#include "precompiled_entities.h"

namespace precompiled_entities {

    void compile() {
        white_sphere_entity = std::make_shared<SphereEntity>(glm::vec3(0.0f), 3, 0.5f, false, white_base_material);
    }


}

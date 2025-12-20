#ifndef PRECOMPILED_ENTITIES_H
#define PRECOMPILED_ENTITIES_H
#include <memory>

#include "mesh/sphere_entity.h"

namespace precompiled_entities {

    inline std::shared_ptr <SphereEntity> white_sphere_entity;

    void compile();

}

#endif //PRECOMPILED_ENTITIES_H

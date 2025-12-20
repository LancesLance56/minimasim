//
// Created by User on 17/09/2025.
//

#ifndef PRECOMPILED_SHADERS_H
#define PRECOMPILED_SHADERS_H

#include <memory>
#include "shader.h"

// shaders

namespace PrecompiledShaders {
    inline std::shared_ptr<Shader> colored_shader;
    inline std::shared_ptr<Shader> phong_shader;
}

#endif //PRECOMPILED_SHADERS_H

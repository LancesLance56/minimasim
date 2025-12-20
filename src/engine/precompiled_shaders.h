//
// Created by User on 17/09/2025.
//

#ifndef PRECOMPILED_SHADERS_H
#define PRECOMPILED_SHADERS_H

#include <memory>
#include "shader.h"

// shaders

namespace precompiled_shaders {
    inline std::shared_ptr<Shader> colored_shader;
    inline std::shared_ptr<Shader> phong_texture_normals_shader;
    inline std::shared_ptr<Shader> phong_normals_shader;
    inline std::shared_ptr<Shader> phong_texture_normals_geom_shader;
    inline std::shared_ptr<Shader> phong_normals_geom_shader;

    void compile();
}

#endif //PRECOMPILED_SHADERS_H

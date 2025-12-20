#include "precompiled_shaders.h"

namespace precompiled_shaders {
    void compile() {
        phong_texture_normals_shader = std::make_shared<Shader>(
    "./src/shaders/phong/phong_texture_normals.vert", "./src/shaders/phong/phong_texture_normals.frag");
        phong_normals_shader = std::make_shared<Shader>(
            "./src/shaders/phong/phong_normals.vert", "./src/shaders/phong/phong_normals.frag");
        phong_texture_normals_geom_shader =std::make_shared<Shader>(
            "./src/shaders/phong/phong_texture_normals.vert", "./src/shaders/geometry_triangle.frag",
            "./src/shaders/geometry_triangle.geom");
        phong_normals_geom_shader = std::make_shared<Shader>(
            "./src/shaders/phong/phong_normals.vert", "./src/shaders/geometry_triangle.frag",
            "./src/shaders/geometry_triangle.geom");
        colored_shader = std::make_shared<Shader>(
            "./src/shaders/color.vert", "./src/shaders/color.frag");
    }
}

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include "glad/glad.h"

#include "material.h"

enum class ShaderType { VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER, SHADER_PROGRAM };


class Shader {

public:
    GLuint ID;

    Shader();
    ~Shader(); // Add this inside the class definition
    Shader(const std::string& path_to_vertex_shader, const std::string& path_to_fragment_shader);
    Shader(const std::string& vs_path, const std::string& fs_path, const std::string& gs_path);
    static void check_shader_compiler_errors(GLuint shader_id, ShaderType shader_type);
    void bind_texture(const std::string& name, GLuint texture) const;
    void set_int(const std::string& name, int value) const;
    void set_float(const std::string& name, float value) const;
    void set_vec3(const std::string& name, const glm::vec3& value) const;
    void set_mat4(const std::string& name, const glm::mat4& mat) const;
    void set_material(const std::string& name, const Material& material) const;
    void set_float_array(const std::string& name, const std::vector<float>& values) const;
    void set_vec3_array(const std::string& name, const std::vector<glm::vec3>& values) const;
    void set_material_array(const std::string& name, const std::vector<Material>& values) const;
    void use() const;
};

#endif

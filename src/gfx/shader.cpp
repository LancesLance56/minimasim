#include "shader.h"

#include <format>
#include <vector>

#include <fstream>
#include <iostream>
#include <sstream>

#include "../../include/glm/gtc/type_ptr.hpp"

Shader::Shader() = default;

Shader::~Shader() {
    if (ID != 0) {
        glDeleteProgram(ID);
    }
}

Shader::Shader(const std::string &path_to_vertex_shader, const std::string &path_to_fragment_shader) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(path_to_vertex_shader);
        fShaderFile.open(path_to_fragment_shader);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }

    catch (std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        std::cout << path_to_vertex_shader << " not found in file" << std::endl;
        std::cout << path_to_fragment_shader << " not found in file" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertexShader;
    unsigned int fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
    glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    check_shader_compiler_errors(vertexShader, ShaderType::VERTEX_SHADER);
    check_shader_compiler_errors(fragmentShader, ShaderType::FRAGMENT_SHADER);

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    check_shader_compiler_errors(ID, ShaderType::SHADER_PROGRAM);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::Shader(const std::string& vs_path, const std::string& fs_path, const std::string& gs_path) {
    std::string vertex_code, fragment_code, geometry_code;
    std::ifstream v_shader_file, f_shader_file, g_shader_file;

    v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    g_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        v_shader_file.open(vs_path);
        f_shader_file.open(fs_path);
        g_shader_file.open(gs_path);

        std::stringstream vShaderStream, fShaderStream, gShaderStream;
        vShaderStream << v_shader_file.rdbuf();
        fShaderStream << f_shader_file.rdbuf();
        gShaderStream << g_shader_file.rdbuf();

        vertex_code = vShaderStream.str();
        fragment_code = fShaderStream.str();
        geometry_code = gShaderStream.str();

        v_shader_file.close();
        f_shader_file.close();
        g_shader_file.close();
    } catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n";
        std::cerr << vs_path << "\n" << fs_path << "\n" << gs_path << "\n";
    }

    const char* v_shader_code = vertex_code.c_str();
    const char* f_shader_code = fragment_code.c_str();
    const char* g_shader_code = geometry_code.c_str();

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);

    glShaderSource(vertex_shader, 1, &v_shader_code, nullptr);
    glShaderSource(fragment_shader, 1, &f_shader_code, nullptr);
    glShaderSource(geometry_shader, 1, &g_shader_code, nullptr);

    glCompileShader(vertex_shader);
    glCompileShader(fragment_shader);
    glCompileShader(geometry_shader);

    check_shader_compiler_errors(vertex_shader, ShaderType::VERTEX_SHADER);
    check_shader_compiler_errors(fragment_shader, ShaderType::FRAGMENT_SHADER);
    check_shader_compiler_errors(geometry_shader, ShaderType::GEOMETRY_SHADER);

    ID = glCreateProgram();
    glAttachShader(ID, vertex_shader);
    glAttachShader(ID, fragment_shader);
    glAttachShader(ID, geometry_shader);

    glLinkProgram(ID);
    check_shader_compiler_errors(ID, ShaderType::SHADER_PROGRAM);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteShader(geometry_shader);
}


// In shader.cpp
void Shader::check_shader_compiler_errors(const GLuint shader_id, const ShaderType shader_type) {
    int success;
    char info_log[512];

    switch (shader_type) {
        case ShaderType::VERTEX_SHADER:
            glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader_id, 512, nullptr, info_log);
                std::cerr << "Error: " << "Vertex "
                          << "Shader Compilation Failed\n"
                          << info_log << "\n";
            } else {
                // Always print info log, even on success, for warnings
                glGetShaderInfoLog(shader_id, 512, nullptr, info_log);
                if (strlen(info_log) > 0) {
                    std::cout << "Info/Warning for "
                              << "Vertex "
                              << "Shader: " << info_log << "\n";
                }
            }
            break;

        case ShaderType::FRAGMENT_SHADER:
            glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader_id, 512, nullptr, info_log);
                std::cerr << "Error: " << "Fragment "
                          << "Shader Compilation Failed\n"
                          << info_log << "\n";
            } else {
                // Always print info log, even on success, for warnings
                glGetShaderInfoLog(shader_id, 512, nullptr, info_log);
                if (strlen(info_log) > 0) {
                    std::cout << "Info/Warning for " << "Fragment " << " Shader: "
                              << info_log << "\n";
                }
            }
            break;

        case ShaderType::GEOMETRY_SHADER:
            glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader_id, 512, nullptr, info_log);
                std::cerr << "Error: Geometry Shader Compilation Failed\n" << info_log << "\n";
            } else {
                glGetShaderInfoLog(shader_id, 512, nullptr, info_log);
                if (strlen(info_log) > 0) {
                    std::cout << "Info/Warning for Geometry Shader:\n" << info_log << "\n";
                }
            }
            break;

        case ShaderType::SHADER_PROGRAM:
            glGetProgramiv(shader_id, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader_id, 512, nullptr, info_log);
                std::cerr << "ID: " << shader_id << "\n";
                std::cerr << "Error: Shader Program Linking Failed\n" << info_log << "\n";
            } else {
                // Always print info log, even on success, for warnings
                glGetProgramInfoLog(shader_id, 512, nullptr, info_log);
                if (strlen(info_log) > 0) {
                    std::cout << "Info/Warning for Shader Program:\n" << info_log << "\n";
                }
            }
            break;
    }
}

void Shader::bind_texture(const std::string &name, const GLuint texture) const {
    this->use();

    glActiveTexture(GL_TEXTURE0 + texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<GLint>(texture));
}

void Shader::set_vec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::set_int(const std::string &name, const int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set_float(const std::string &name, const float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set_mat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::set_material(const std::string &name, const Material &material) const {
    glUniform3fv(glGetUniformLocation(ID, (name + ".ambient").c_str()), 1, &material.ambient[0]);
    glUniform3fv(glGetUniformLocation(ID, (name + ".diffuse").c_str()), 1, &material.diffuse[0]);
    glUniform3fv(glGetUniformLocation(ID, (name + ".specular").c_str()), 1, &material.specular[0]);
    glUniform1f(glGetUniformLocation(ID, (name + ".shininess").c_str()), material.shininess);
}

void Shader::set_float_array(const std::string& name, const std::vector<float>& values) const {
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location != -1) {
        glUniform1fv(location, values.size(), values.data());
    } else {
        std::cerr << "Warning: Float array uniform '" << name << "' not found.\n";
    }
}

void Shader::set_vec3_array(const std::string& name, const std::vector<glm::vec3>& values) const {
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location != -1) {
        glUniform3fv(location, values.size(), glm::value_ptr(values[0]));
    } else {
        std::cerr << "Warning: Vec3 array uniform '" << name << "' not found.\n";
    }
}

void Shader::set_material_array(const std::string &name, const std::vector<Material> &values) const {
    if (GLint location = glGetUniformLocation(ID, (std::format("{}[{}]{}", name, 0, ".ambient").c_str()));
        location != -1) {
        for (int i = 0; i < values.size(); ++i) {
            auto variable_name = std::format("{}[{}]", name, i); // "<name>[i]"
            glUniform3fv(glGetUniformLocation(ID, (variable_name + ".ambient").c_str()), 1, &values[i].ambient[0]);
            glUniform3fv(glGetUniformLocation(ID, (variable_name + ".diffuse").c_str()), 1, &values[i].diffuse[0]);
            glUniform3fv(glGetUniformLocation(ID, (variable_name + ".specular").c_str()), 1, &values[i].specular[0]);
            glUniform1f(glGetUniformLocation(ID, (variable_name + ".shininess").c_str()), values[i].shininess);
        }
    } else {
        std::cerr << "Warning: Material array uniform '" << name << "' not found.\n";
    }
}



void Shader::use() const {
    if (ID == 0) {
        std::cerr << "[WARN] Tried to use shader program before initialization.\n";
        return;
    }
    glUseProgram(ID);
}


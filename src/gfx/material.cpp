#include "material.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "shader.h"

Material::Material()
    : ambient(0.1f), diffuse(1.0f), specular(1.0f), shininess(32.0f) {}

Material::Material(const glm::vec3& ambient,
                   const glm::vec3& diffuse,
                   const glm::vec3& specular,
                   const float shininess)
    : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}

 Material::Material(const glm::vec3 &rgb, float shininess) : ambient(rgb), diffuse(rgb), specular(rgb), shininess(shininess) {}


void Material::apply_to_shader(const Shader& shader, const std::string& prefix) const {
    shader.set_vec3(prefix + ".ambient", ambient);
    shader.set_vec3(prefix + ".diffuse", diffuse);
    shader.set_vec3(prefix + ".specular", specular);
    shader.set_float(prefix + ".shininess", shininess);
}

void Material::display_dear_imgui() {
    ImGui::ColorEdit3("Ambient", glm::value_ptr(ambient));
    ImGui::ColorEdit3("Diffuse", glm::value_ptr(diffuse));
    ImGui::ColorEdit3("Specular", glm::value_ptr(specular));
    ImGui::SliderFloat("Shininess",& shininess, 0.0f, 128.0f);
}

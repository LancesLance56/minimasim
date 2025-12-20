#ifndef CUBE_H
#define CUBE_H

#include "mesh.h"
#include "../engine/entity.h"

struct Light;

class CubeEntity : public Entity {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 color_rgb = { 1.0f, 1.0f, 1.0f };
    std::string texture_path;
    Material phong_cube_material = base_material;
    Mesh colored_cube_mesh;
    Mesh phong_cube_mesh;
    float size = 0.0f;

    explicit CubeEntity(glm::vec3 position, glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), float size = 2, glm::vec3 rgb = glm::vec3(1.0f, 1.0f, 1.0f),
            const std::string &texture_path = "./src/textures/Tileable noise.png"); // not sdt::string& fix later
    void draw(const Camera &camera, const RenderSettings &render_settings,
         const std::span<const Light> &light_entities) override;
    void draw(const Camera &camera, const RenderSettings &render_settings) override;
    void update(double dt) override;
};

#endif

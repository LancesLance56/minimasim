#ifndef CUBE_H
#define CUBE_H

#include "../engine/entity/components/mesh_filter_component.h"
#include "../engine/entity/entity.h"

struct Light;

class BaseCubeEntity : public Entity<BaseCubeEntity> {
public:
    glm::vec3 color_rgb = {1.0f, 1.0f, 1.0f};

    explicit BaseCubeEntity(
            glm::vec3 position, glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f),
            glm::vec3 rgb = glm::vec3(1.0f)) : color_rgb(rgb) {
        transform.position = position;
        transform.rotation = rotation;
        transform.scale = scale;
    }

    ~BaseCubeEntity() override = default;
    void update(double dt) override {}
};

class ColoredCubeEntity : public BaseCubeEntity {
public:
    explicit ColoredCubeEntity(glm::vec3 position,
                               glm::vec3 rotation = glm::vec3(0.0f),
                               glm::vec3 scale = glm::vec3(1.0f),
                               glm::vec3 rgb = glm::vec3(1.0f));

    void draw(const Camera& camera, const RenderSettings& render_settings) override;
};

class PhongCubeEntity : public BaseCubeEntity {
public:
    Material material = blue_base_material;

    explicit PhongCubeEntity(glm::vec3 position,
                             glm::vec3 rotation = glm::vec3(0.0f),
                             glm::vec3 scale = glm::vec3(1.0f),
                             glm::vec3 rgb = glm::vec3(1.0f),
                             const std::string &texture_path = "");

    void draw(const Camera& camera, const RenderSettings &render_settings,
              const std::span<const Light> &light_entities) override;

    void on_gui(int entity_id) override;
};

#endif

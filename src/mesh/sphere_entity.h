#ifndef SPHERE_ENTITY_H
#define SPHERE_ENTITY_H

#include "../engine/entity/components/mesh_filter_component.h"
#include "../engine/entity/components/mesh_renderer_component.h"
#include "engine/entity/entity.h"
#include "gfx.h"
#include "math/n_body_sim.h"

class SphereEntity : public Entity<SphereEntity> {
public:
    Material material;

    float radius = 1.0f;

    std::optional<NBodySim::MassData> mass_attr;

    explicit SphereEntity(glm::vec3 position,
                          int subdivisions = 3,
                          float radius = 1.0f,
                          bool has_normals = true,
                          const Material &material = blue_base_material);

    void draw(const Camera& cam, const RenderSettings &rs);
    void draw(const Camera &cam, const RenderSettings &rs, const std::span<const Light> &lights);

    void on_gui(int entity_id) override;

    void update(double dt) override {} // no-op

    [[nodiscard]] glm::mat4 compute_model_matrix() const;
};

#endif

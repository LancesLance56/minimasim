#ifndef AXIS_ENTITY_H
#define AXIS_ENTITY_H

#include "../engine/entity/entity.h"
#include "shader.h"

class AxisEntity final : public Entity<AxisEntity> {
public:
    explicit AxisEntity(float axis_length = 5.0f, float arrow_size = 0.2f);
    ~AxisEntity() override;

    void update(double dt) override;
    void draw(const Camera& camera, const RenderSettings& settings) override;

private:
    GLuint vao;
    GLuint vbo;
    Shader shader;

    float axis_length;
    float arrow_size;

    void setup_buffers();
};

#endif // AXIS_ENTITY_H

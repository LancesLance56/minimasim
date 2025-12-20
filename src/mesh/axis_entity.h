#ifndef AXIS_ENTITY_H
#define AXIS_ENTITY_H

#include "engine/entity.h"
#include "shader.h"

class AxisEntity : public Entity {
public:
    explicit AxisEntity(float axis_length = 5.0f, float arrow_size = 0.2f);
    ~AxisEntity() override;

    void update(double dt) override;
    void draw(const Camera& camera, const RenderSettings& settings, const std::span<const Light> &lights) override;

private:
    GLuint vao_;
    GLuint vbo_;
    Shader shader_;

    float axis_length_;
    float arrow_size_;

    void setup_buffers();
};

#endif // AXIS_ENTITY_H

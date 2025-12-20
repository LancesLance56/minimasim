#ifndef ENTITY_H
#define ENTITY_H

#include "../entity_base.h"
#include "../render_settings.h"

class Entity : public EntityBase {
public:
    RenderSettings render_settings;

    RenderSettings& get_render_settings() override {
        return render_settings;
    }
};

#endif

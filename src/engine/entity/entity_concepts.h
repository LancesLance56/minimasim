#ifndef ENTITY_CONCEPTS_H
#define ENTITY_CONCEPTS_H

#include <span>
#include "camera.h"
#include "engine/render_settings.h"
#include "light.h"

template <typename T>
concept HasUnlitDraw = requires(T t, Camera c, RenderSettings rs) {
    { t.draw(c, rs) };
};

template <typename T>
concept HasLitDraw = requires(T t, Camera c, RenderSettings rs, std::span<const Light> lights) {
    { t.draw(c, rs, lights) };
};

template <typename T>
concept HasAnyDraw = HasUnlitDraw<T> || HasLitDraw<T>;

#endif

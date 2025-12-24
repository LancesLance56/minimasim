#ifndef GFX_H
#define GFX_H

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

#include "material.h"

// values from 0 to 1
struct Color {
    float red;
    float green;
    float blue;
    float alpha = 1.0f;
};

const Color SKY_BLUE = { 0.529, 0.808, 0.922 };

const auto blue_base_material = Material(
    glm::vec3(0.1f, 0.65f, 08.5f),
    glm::vec3(1.0f),
    glm::vec3(0.5f),
    32.0f
);

const auto gray_base_material = Material(
    glm::vec3(0.5f, 0.5f, 0.5f),
    glm::vec3(1.0f),
    glm::vec3(0.3f),
    32.0f
);

const auto orange_base_material = Material(
    glm::vec3(1.0f, 0.4f, 0.1f),
    glm::vec3(1.0f),
    glm::vec3(0.5f),
    32.0f
);

const auto yellow_base_material = Material(
    glm::vec3(1.0f, 1.0f, 0.0f),
    glm::vec3(1.0f),
    glm::vec3(0.5f),
    32.0f
);
const auto white_base_material = Material(
    glm::vec3(1.0f),
    glm::vec3(1.0f),
    glm::vec3(0.5f),
    32.0f
);

const auto grass_material = Material(
    glm::vec3(0.05f, 0.2f, 0.05f),   // ambient
    glm::vec3(0.1f, 0.6f, 0.1f),    // diffuse
    glm::vec3(0.05f, 0.3f, 0.05f),  // specular
    16.0f                           // shininess
);

const auto dirt_material = Material(
    glm::vec3(0.15f, 0.1f, 0.05f),
    glm::vec3(0.4f, 0.3f, 0.2f),
    glm::vec3(0.1f, 0.1f, 0.1f),
    8.0f
);

const auto rock_material = Material(
    glm::vec3(0.2f, 0.2f, 0.2f),
    glm::vec3(0.4f, 0.4f, 0.4f),
    glm::vec3(0.3f, 0.3f, 0.3f),
    32.0f
);

const auto snow_material = Material(
    glm::vec3(0.8f, 0.8f, 0.8f),
    glm::vec3(0.95f, 0.95f, 0.95f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    64.0f
);

const auto water_material = Material(
    glm::vec3(0.0f, 0.05f, 0.1f),
    glm::vec3(0.0f, 0.3f, 0.6f),
    glm::vec3(0.2f, 0.4f, 0.7f),
    128.0f
);

const auto sand_material = Material{
    glm::vec3(0.80f, 0.75f, 0.45f),  // ambient (more yellow)
    glm::vec3(0.98f, 0.92f, 0.55f),  // diffuse (more yellow)
    glm::vec3(0.22f, 0.22f, 0.10f),  // specular (more yellow)
    8.0f                             // shininess (low reflectivity)
};


#endif

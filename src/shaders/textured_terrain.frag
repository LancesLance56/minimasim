#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
};

#define MAX_LIGHTS 4
#define MAX_TERRAIN_TYPES 7

uniform Light lights[MAX_LIGHTS];
uniform int lightCount;

uniform vec3 viewPos;

uniform int terrainTypeCount;
uniform float terrainHeights[MAX_TERRAIN_TYPES];
uniform sampler2D terrainTextures[MAX_TERRAIN_TYPES];
uniform Material terrainMaterials[MAX_TERRAIN_TYPES];

uniform float heightMultiplier;
uniform float textureWrapScale;
uniform float yMax;
uniform float yMin;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in float WorldHeight;

out vec4 FragColor;

// Gamma value (sRGB)
const float gamma = 2.2;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0);

    // Normalize height
    float heightNormalized = (WorldHeight - yMin) / (yMax - yMin);

    // Blend textures based on height
    vec3 blendedTexColor = vec3(0.0);
    Material currentMaterial = terrainMaterials[0]; // Default

    for (int i = 0; i < terrainTypeCount - 1; ++i) {
        float lowerHeight = terrainHeights[i];
        float upperHeight = terrainHeights[i + 1];

        if (heightNormalized >= lowerHeight && heightNormalized < upperHeight) {
            float blendFactor = smoothstep(lowerHeight, upperHeight, heightNormalized);
            vec3 tex1Color = texture(terrainTextures[i], TexCoord * textureWrapScale).rgb;
            vec3 tex2Color = texture(terrainTextures[i+1], TexCoord * textureWrapScale).rgb;
            blendedTexColor = mix(tex1Color, tex2Color, blendFactor);
        }
        if (heightNormalized >= terrainHeights[terrainTypeCount - 1]) {
            blendedTexColor = texture(terrainTextures[terrainTypeCount - 1], TexCoord * textureWrapScale).rgb;
        }
    }

    // Pick material based on height
    int selectedMaterialID = 0;
    if (heightNormalized <= terrainHeights[0]) {
        selectedMaterialID = 0;
    } else if (heightNormalized >= terrainHeights[terrainTypeCount - 1]) {
        selectedMaterialID = terrainTypeCount - 1;
    } else {
        for (int i = 1; i < terrainTypeCount; ++i) {
            if (heightNormalized <= terrainHeights[i]) {
                selectedMaterialID = i;
                break;
            }
        }
    }
    currentMaterial = terrainMaterials[selectedMaterialID];

    // --- Phong/Blinn lighting ---
    for (int i = 0; i < lightCount; ++i) {
        vec3 lightDir = normalize(lights[i].position - FragPos);
        float distance = length(lights[i].position - FragPos);

        // Attenuation
        float attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance +
        lights[i].quadratic * (distance * distance));

        // Ambient
        vec3 ambient = currentMaterial.ambient * lights[i].color;

        // Diffuse
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = currentMaterial.diffuse * diff * lights[i].color;

        // Blinn-Phong specular
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(norm, halfwayDir), 0.0), currentMaterial.shininess);
        vec3 specular = currentMaterial.specular * spec * lights[i].color;

        // Apply intensity & attenuation
        vec3 lightContribution = (ambient + diffuse + specular) * lights[i].intensity * attenuation;
        result += lightContribution;
    }

    // Optional debug tint
    vec3 debugNormalColor = 0.05 * norm;
    result += debugNormalColor;

    // Apply texture
    result *= blendedTexColor;

    // Gamma correction (linear -> sRGB)
    result = pow(result, vec3(1.0 / gamma));

    FragColor = vec4(result, 1.0);
}

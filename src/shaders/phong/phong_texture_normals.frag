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
};

#define MAX_LIGHTS 4
uniform Light lights[MAX_LIGHTS];
uniform int lightCount;

uniform Material material;

uniform vec3 viewPos;

uniform bool shouldDrawBaseTexture;
uniform sampler2D baseTexture;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

// Approximate gamma correction (fast)
vec3 toSRGBFast(vec3 linear) {
  return sqrt(linear); // approximates gamma 2.0 instead of 2.2
  // return pow(linear, 2.2) // more acc
}

void main()
{
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);

  vec3 lighting = vec3(0.0);

  for (int i = 0; i < lightCount; ++i) {
    // Distance and attenuation
    float distance = length(lights[i].position - FragPos);
    float constant = 1.0;
    float linearAtt = 0.09;
    float quadratic = 0.032;
    float attenuation = 1.0 / (constant + linearAtt * distance + quadratic * distance * distance);

    vec3 lightContribution = lights[i].color * lights[i].intensity * attenuation;

    // Ambient
    vec3 ambient = material.ambient * lightContribution;

    // Diffuse
    vec3 lightDir = normalize(lights[i].position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = material.diffuse * diff * lightContribution;

    // Blinn-Phong specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = material.specular * spec * lightContribution;

    lighting += ambient + diffuse + specular;
  }

  if (shouldDrawBaseTexture) {
    vec3 texColor = texture(baseTexture, TexCoord).rgb;
    FragColor = vec4(clamp(lighting * texColor, 0.0, 1.0), 1.0);
  } else {
    FragColor = vec4(clamp(lighting, 0.0, 1.0), 1.0);
  }
}


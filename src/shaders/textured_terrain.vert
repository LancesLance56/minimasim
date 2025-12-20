#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;

out VS_OUT {
    vec3 pos;
    vec3 normal;
} vs_out;

uniform mat4 mvp;
uniform mat4 model = mat4(1.0f);

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out float WorldHeight;

void main() {
    vec3 pos = inPosition; // Y is already remapped on CPU

    FragPos = vec3(model * vec4(pos, 1.0));
    Normal = mat3(transpose(inverse(model))) * inNormal;
    TexCoord = inTexCoord;
    WorldHeight = pos.y;
    vs_out.pos = FragPos;
    vs_out.normal = Normal;
    gl_Position = mvp * vec4(pos, 1.0);
}

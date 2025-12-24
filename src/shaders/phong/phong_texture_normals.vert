#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 mvp;
uniform mat4 model;

out VS_OUT {
    vec3 pos;
    vec3 normal;
} vs_out;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;
    vs_out.normal = Normal;
    vs_out.pos = FragPos;
    gl_Position = mvp * model * vec4(aPos, 1.0);
}

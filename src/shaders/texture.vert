#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texture_coord_attr;

out vec2 texture_coordinate;

uniform mat4 mvp;

void main() {
    texture_coordinate = texture_coord_attr;

    gl_Position = mvp * vec4(aPos, 1.0);
}

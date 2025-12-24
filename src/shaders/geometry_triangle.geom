#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

in VS_OUT {
    vec3 pos;
    vec3 normal;
} gs_in[];

uniform mat4 mvp;
uniform float normalLength = 0.1;

out vec3 gsColor; // Pass to fragment shader

void main() {
    // Compute triangle center
    vec3 center = (gs_in[0].pos + gs_in[1].pos + gs_in[2].pos) / 3.0;

    // Compute face normal (cross product of edges)
    vec3 edge1 = gs_in[1].pos - gs_in[0].pos;
    vec3 edge2 = gs_in[2].pos - gs_in[0].pos;
    vec3 normal = normalize(cross(edge1, edge2));

    // Optional: flip normal if winding is clockwise
    // normal = normalize(normal);

    // Set line color to bright red
    gsColor = vec3(1.0, 0.0, 0.0);

    // Emit start vertex (center)
    gl_Position = mvp * vec4(center, 1.0);
    EmitVertex();

    // Emit end vertex (along normal)
    gl_Position = mvp * vec4(center + normal * normalLength, 1.0);
    EmitVertex();

    EndPrimitive();
}

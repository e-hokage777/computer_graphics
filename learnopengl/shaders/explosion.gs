#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 texCoords;
} gs_in[];

out vec2 TexCoords;

uniform float time;

vec3 GetNormal() {
    vec4 p1 = gl_in[0].gl_Position;
    vec4 p2 = gl_in[1].gl_Position;
    vec4 p3 = gl_in[2].gl_Position;

    vec3 a = vec3(p1 - p2);
    vec3 b = vec3(p1 - p3);
    return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal) {
    float mangitude = 2.0;
    vec3 direction = normal * mangitude * (sin(time) + 1.0);
    return position + vec4(direction, 0.0);
}

void main() {
    vec3 normal = GetNormal();
    gl_Position = explode(gl_in[0].gl_Position, normal);
    TexCoords = gs_in[0].texCoords;
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    TexCoords = gs_in[1].texCoords;
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    TexCoords = gs_in[2].texCoords;
    EmitVertex();
    EndPrimitive();
}
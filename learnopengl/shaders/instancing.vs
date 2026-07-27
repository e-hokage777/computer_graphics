#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 2) in vec2 offset;

out VS_OUT {
    vec3 color;
} vs_out;

// uniform vec2 offsets[100];

void main() {
    vs_out.color = pos;
    vec3 apos = pos * gl_InstanceID/100;
    gl_Position = vec4(apos, 1.0) + vec4(offset, 0.0, 0.0);
}
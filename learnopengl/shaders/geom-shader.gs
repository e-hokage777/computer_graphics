#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 5) out;

in VS_OUT {vec3 color;} gs_in[];

out vec3 fragColor;

void buildHouse(vec4 position) {
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(0, 0.4, 0.0, 0.0);
    fragColor = vec3(1.0, 1.0, 1.0);
    EmitVertex();
    EndPrimitive();
}

void main() {
    // gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);
    // EmitVertex();
    // gl_Position = gl_in[0].gl_Position + vec4(0.1, 0.0, 0.0, 0.0);
    // EmitVertex();
    // EndPrimitive();
    fragColor = gs_in[0].color;
    buildHouse(gl_in[0].gl_Position);
}
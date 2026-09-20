#version 330 core

in vec4 FragPos;


uniform float farPlane;
uniform vec3 lightPos;

void main(){
    float lightDistance = length(lightPos - FragPos.xyz);

    gl_FragDepth = lightDistance/farPlane;
}
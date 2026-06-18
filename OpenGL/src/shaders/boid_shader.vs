#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vColor;

out vec3 fColor;

uniform float uTime;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const float FLAP_SPEED = 5.0;
const float FLAP_AMPLITUDE = 0.5;
const float FLAP_OFFSET = 0.5;

void main() {
    float flapAmp = aPos.x * FLAP_AMPLITUDE;
    float flapOffset = aPos.x * FLAP_OFFSET;

    float flap = sin((uTime * FLAP_SPEED) + flapOffset) * flapAmp;
    vec3 flapPos = vec3(aPos.x, aPos.y + (flap * 0.5), aPos.z + flap);

    gl_Position = projection * view * model * vec4(aPos, 1.0);

    fColor = vColor;
}
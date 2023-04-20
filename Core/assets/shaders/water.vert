#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT
{
    vec3 position;
    vec3 normal;
    vec2 uv;
} vs_out;

uniform mat4 PV;

void main()
{
    gl_Position = PV * vec4(aPos, 1.0);

    vs_out.position = aPos;
    vs_out.normal = aNormal;
    vs_out.uv = aTexCoords;
}
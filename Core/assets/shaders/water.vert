#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT
{
    vec3 position;
    vec3 normal;
    vec2 uv;
} vs_out;

layout (std140, binding = 1) uniform Matrices
{
    mat4 PROEJECTION_MATRIX;
    mat4 VIEW_MATRIX;
    mat4 PV_MATRIX;
};

uniform mat4 MODEL_MATRIX;

void main()
{
    gl_Position = PV_MATRIX * MODEL_MATRIX * vec4(aPos, 1.0);

    vs_out.position = aPos;
    vs_out.uv = aTexCoords;
}
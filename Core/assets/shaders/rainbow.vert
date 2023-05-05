#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;

layout (std140, binding = 1) uniform Matrices
{
    mat4 PV_MATRIX;
};

uniform mat4 MODEL_MATRIX;

void main()
{
    gl_Position = PV_MATRIX * MODEL_MATRIX * vec4(aPos, 1.0);
    Normal = mat3(transpose(inverse(MODEL_MATRIX))) * aNormal;
}
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aInstanceMatrix;
// location 4, 5, 6 are reserved for instanceMatrix

out vec2 TexCoords;

layout (std140, binding = 1) uniform Matrices
{
    uniform mat4 PV_MATRIX;
};

void main()
{
    TexCoords = aTexCoords;

    gl_Position = PV_MATRIX * aInstanceMatrix * vec4(aPos, 1.0);
}
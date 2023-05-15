#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;

out float height;
out vec2 TexCoords;

layout (std140, binding = 1) uniform Matrices
{
    mat4 PV_MATRIX;
    mat4 VIEW_MATRIX;
    mat4 MODEL_MATRIX;
};

void main()
{   
    height = aPos.y;
    TexCoords = aTexCoords;

    gl_Position = PV_MATRIX * MODEL_MATRIX * vec4(aPos, 1.0);
}
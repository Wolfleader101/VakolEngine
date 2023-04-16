#version 460 core
layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

layout (std140, binding = 1) uniform Matrices
{   
    mat4 PV_MATRIX;
};

void main()
{
    TexCoords = aPos;
    vec4 pos = PV_MATRIX * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
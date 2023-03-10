#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 uv;

void main()
{
    uv = vec2(aTexCoords.x, aTexCoords.y);
    gl_Position = vec4(aPos, 1.0);
}
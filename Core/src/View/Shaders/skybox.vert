#version 460 core
layout(location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;

out vec2 uv;

void main()
{
    uv.x = aPos.x;
    uv.y = aPos.y;

    gl_Position = projection * view * vec4(aPos, 1.0);
}
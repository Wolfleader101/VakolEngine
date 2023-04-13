#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 _PVM;

void main()
{   
    gl_Position = _PVM * vec4(aPos, 1.0);
}
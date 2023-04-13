#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 _PV;

void main()
{   
    gl_Position = _PV * vec4(aPos, 1.0);
}
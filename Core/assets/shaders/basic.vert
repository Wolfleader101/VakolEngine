#version 330

layout (location = 0) in vec3 pos;

uniform mat4 _PV;

void main()
{   
    gl_Position = _PV * vec4(pos, 1.0);
}
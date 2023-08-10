#version 460 core
layout(location = 0) in vec3 aPos;

uniform mat4 PV_MATRIX;

void main()
{
	gl_Position = PV_MATRIX * vec4(aPos, 1.0);
}
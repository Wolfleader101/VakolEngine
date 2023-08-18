#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 PV_MATRIX;
uniform mat4 MODEL_MATRIX;

void main()
{
	TexCoords = aTexCoords;

	gl_Position = PV_MATRIX * MODEL_MATRIX * vec4(aPos, 1.0);
}
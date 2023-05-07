#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D diffuse_map;
uniform sampler2D specular_map;
uniform sampler2D normal_map;
uniform sampler2D emission_map;

void main()
{
	FragColor = texture(diffuse_map, TexCoords);
}
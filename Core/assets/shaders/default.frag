#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

struct Material
{
	vec3 ambient_color;
	vec3 diffuse_color;
	vec3 specular_color;

	sampler2D diffuse_texture;
	sampler2D specular_texture;
	sampler2D ambient_texture;
	sampler2D emission_texture;
	sampler2D height_texture;
	sampler2D normal_texture;
};

uniform Material material;

void main()
{
	vec4 ambient = vec4(material.ambient_color, 1.0);
	vec4 diffuse = vec4(material.diffuse_color, 1.0);
	vec4 specular = vec4(material.specular_color, 1.0);

	FragColor = ambient + diffuse;
}
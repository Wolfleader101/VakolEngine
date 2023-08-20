#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

struct Material
{
	vec3 ambient_color;
	vec3 diffuse_color;
	vec3 specular_color;
	vec3 emissive_color;

    sampler2D diffuse_map;
	sampler2D specular_map;
	sampler2D ambient_map;
	sampler2D emissive_map;
	sampler2D height_map;
	sampler2D normal_map;
};

uniform Material material;

void main()
{
	vec4 diffuse = texture(material.diffuse_map, TexCoords);
	vec4 specular = texture(material.specular_map, TexCoords);
	vec4 ambient = texture(material.ambient_map, TexCoords);
	vec4 normal = texture(material.normal_map, TexCoords);

	FragColor = diffuse;
}
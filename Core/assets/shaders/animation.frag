#version 460 core
out vec4 FragColor;

in VS_OUT
{
	vec3  normal;
    vec2  uv;
} fs_in;

uniform sampler2D diffuse_map;
uniform sampler2D specular_map;
uniform sampler2D normal_map;
uniform sampler2D emission_map;

uniform vec3 tint = vec3(1.0);

void main()
{
	FragColor = vec4(tint, 1.0);
}
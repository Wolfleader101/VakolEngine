#version 460 core
out vec4 FragColor;

in VS_OUT
{
	vec3  normal;
    vec2  uv;
	flat ivec4 bone_ids;
	vec4 bone_weights;
} fs_in;

uniform sampler2D diffuse_map;
uniform sampler2D specular_map;
uniform sampler2D normal_map;
uniform sampler2D emission_map;

void main()
{
	FragColor = texture(diffuse_map, fs_in.uv);
}
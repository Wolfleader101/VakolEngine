#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D diffuse_map;
uniform vec2 uv_offset = vec2(0.0);

void main()
{
    FragColor = texture(diffuse_map, TexCoords + uv_offset);
}
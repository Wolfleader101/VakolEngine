#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D diffuse_texture;

void main()
{
    FragColor = texture(diffuse_texture, TexCoords);
}
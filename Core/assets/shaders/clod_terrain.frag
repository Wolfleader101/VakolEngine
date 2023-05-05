#version 460 core

out vec4 FragColor;

in float height;
in vec2 TexCoord;
in vec4 FragPos;

void main()
{
    FragColor = vec4(0.12549,0.176471,0.258824, 0.5);
}
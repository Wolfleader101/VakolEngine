#version 460 core
out vec4 FragColor;

in vec4 FinalColor;

uniform vec4 rgba;

void main()
{
    FragColor = FinalColor * rgba;
}
#version 460 core
out vec4 FragColor;

in vec3 FinalColor;

uniform vec3 rgba;

void main()
{
    FragColor = vec4(FinalColor * rgba, 1.0);
}
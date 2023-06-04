#version 460 core

in float Height;

out vec4 FragColor;

uniform float u_time; // Added this uniform variable

void main()
{
    float red = sin(u_time * 0.1) * 0.4 + 0.5;
    float green = sin(u_time * 0.2 + 3.14159 / 2.0) * 0.4 + 0.5;
    float blue = sin(u_time * 0.3 + 3.14159) * 0.4 + 0.5;
    FragColor = vec4(red, green, blue, 1.0);
}
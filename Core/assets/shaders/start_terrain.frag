#version 460 core

in float Height;

out vec4 FragColor;

uniform float time;

void main()
{
    float r = sin(time); // Red.   Value between 0 and 1.
    float g = (sin(time + 2.0944) * 0.5) + 0.5; // Green. Value between 0 and 1. Phase shifted by 2pi/3
    float b = (sin(time + 4.1888) * 0.5) + 0.5; // Blue.  Value between 0 and 1. Phase shifted by 4pi/3

    FragColor = vec4(r, g, b, 1.0);
}
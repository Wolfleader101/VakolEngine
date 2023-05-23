#version 460 core

// In variables
in vec3 color;

// Out variables
out vec4 FragColor;

void main() 
{
   FragColor = vec4(color, 1.0);
}
#version 460 core
out vec4 FragColor;

in float height;
in vec2 TexCoords;

void main()
{
     float h = (height + 16.0) / 32.0;
     
     FragColor = vec4(h, h, h, 1.0);
}
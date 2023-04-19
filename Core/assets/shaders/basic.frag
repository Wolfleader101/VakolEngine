#version 460 core
out vec4 FragColor;

in float height;

uniform vec3 tint = vec3(1.0);

void main()
{
     float h = (height + 16.0) / 32.0;

     FragColor = vec4(h, h, h, 1.0);
}
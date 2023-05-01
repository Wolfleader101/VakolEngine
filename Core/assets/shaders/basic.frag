#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse;
uniform vec3 tint = vec3(1.0);

void main()
{
     FragColor = vec4(tint, 1.0);
}
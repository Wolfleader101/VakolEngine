#version 460 core

in float height;
in vec2 TexCoords;

out vec4 FragColor;

void main()
{
    float hue = height;
    vec3 color = vec3(0.0);
    color.r = clamp(abs(mod(hue * 6.0 + 2.0, 6.0) - 3.0) - 1.0, 0.0, 1.0);
    color.g = clamp(abs(mod(hue * 6.0 + 4.0, 6.0) - 3.0) - 1.0, 0.0, 1.0);
    color.b = clamp(abs(mod(hue * 6.0, 6.0) - 3.0) - 1.0, 0.0, 1.0);

    FragColor = vec4(color, 1.0);
}
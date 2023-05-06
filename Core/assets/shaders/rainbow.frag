#version 460 core

in vec3 Normal;

out vec4 FragColor;

void main()
{
    vec3 normalizedNormal = normalize(Normal) * 0.5 + 0.5;
    float hue = normalizedNormal.x;

    vec3 color = vec3(0.0);
    color.r = clamp(abs(mod(hue * 6.0 + 2.0, 6.0) - 3.0) - 1.0, 0.0, 1.0);
    color.g = clamp(abs(mod(hue * 6.0 + 4.0, 6.0) - 3.0) - 1.0, 0.0, 1.0);
    color.b = clamp(abs(mod(hue * 6.0, 6.0) - 3.0) - 1.0, 0.0, 1.0);

    FragColor = vec4(color, 1.0);
}
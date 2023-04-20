#version 460 core

out vec4 FragColor;

uniform sampler2D height_map;

uniform sampler2D water_layer_1;
uniform sampler2D water_layer_2;

uniform sampler2D layer_1;
uniform sampler2D layer_2;

uniform sampler2D layer_3;
uniform sampler2D layer_4;

uniform sampler2D layer_5;
uniform sampler2D layer_6;

layout (std140, binding = 3) uniform Time
{
    float time;
};

in float height;
in vec2 TexCoord;
in vec4 FragPos;

uniform vec4 water_tint = vec4(0.0, 0.4, 1.0, 1.0);

uniform float water_scale_1 = 0.5;
uniform float water_scale_2 = 0.5;

uniform float texture_speed_1 = 0.1;
uniform float texture_speed_2 = 0.08;

uniform float wobbleX = 1.4;
uniform float wobbleY = 1.7;

uniform bool enable_fog = false;

uniform float FOG_DENSITY = 0.025;
uniform vec4 FOG_COLOR = vec4(1.0, 1.0, 1.0, 0.0);

const float level_1 = 60.0;
const float level_2 = 80.0;
const float level_3 = 100.0;
const float level_4 = 150.0;
const float level_5 = 170.0;
const float level_6 = 200.0;

float calculate_fog(float fogCoords)
{
    float result = 0.0;

    // result = exp(-density * fogCoords); // Equation 1

    result = exp(-pow(FOG_DENSITY * fogCoords, 2.0)); // Equation 2

    return 1.0 - clamp(result, 0.0, 1.0);
}

void main()
{
    vec4 water_tex_1 = texture(water_layer_1, vec2(TexCoord.x + (time * texture_speed_1), TexCoord.y + (cos(time) * texture_speed_1)) + cos(time * vec2(wobbleX, wobbleY) + TexCoord.xy * 10.0) * 0.02 * 1.0);
    vec4 water_tex_2 = texture(water_layer_2, vec2(TexCoord.x + (sin(time) * texture_speed_2), TexCoord.y + (time * texture_speed_2)) + cos(time * vec2(wobbleX, wobbleY) + TexCoord.xy * 10.0) * 0.02 * 1.0);

    float g = (height + 16) / 64.0;
    float h = g * 255.0;

    vec4 result = vec4(vec3(0.0), 1.0);
    vec4 water = water_tint + (water_tex_1 * water_scale_1) + (water_tex_2 * water_scale_2);
    
    vec4 greyscale = vec4(g, g, g, 1.0);

    vec4 color_1 = texture(layer_1, TexCoord);
    vec4 color_2 = texture(layer_2, TexCoord);
    vec4 color_3 = texture(layer_3, TexCoord);
    vec4 color_4 = texture(layer_4, TexCoord);
    vec4 color_5 = texture(layer_5, TexCoord);
    vec4 color_6 = texture(layer_6, TexCoord);

    if (h < level_1)
    {
        //result = water * vec4(0.5);
        result = ((water + 0.01) + color_1) * greyscale;
    }
    else if (h < level_2)
    {
        float delta = level_2 - level_1;
        float factor = (h - level_1) / delta;

        result = mix(color_1, color_2, factor) * greyscale;
    }
    else if (h < level_3)
    {
        float delta = level_3 - level_2;
        float factor = (h - level_2) / delta;

        result = mix(color_2, color_3, factor) * greyscale;
    }
    else if (h < level_4)
    {
        float delta = level_4 - level_3;
        float factor = (h - level_3) / delta;

        result = mix(color_3, color_4, factor) * greyscale;
    }
    else if (h < level_5)
    {
        float delta = level_5 - level_4;
        float factor = (h - level_4) / delta;

        result = mix(color_4, color_5, factor) * greyscale;
    }
    else if (h < level_6)
    {
        float delta = level_6 - level_5;
        float factor = (h - level_5) / delta;

        result = mix(color_5, color_6, factor) * greyscale;
    }
    else
        result = color_6 * greyscale;

    if (enable_fog)
    {
        float fogCoords = abs(FragPos.z / FragPos.w);
        FragColor = mix(result, FOG_COLOR, calculate_fog(fogCoords));
    }
    else
        FragColor = result;
}
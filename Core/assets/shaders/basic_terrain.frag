#version 460 core
out vec4 FragColor;

in vec2 TexCoords;
in float Height;

uniform sampler2D layer_1;
uniform sampler2D layer_2;
uniform sampler2D layer_3;
uniform sampler2D layer_4;
uniform sampler2D layer_5;
uniform sampler2D layer_6;

uniform vec2 uv_scale = vec2(20.0);

const float level_1 = 15.0;
const float level_2 = 35.0;
const float level_3 = 55.0;
const float level_4 = 75.0;
const float level_5 = 100.0;
const float level_6 = 125.0;

void main()
{
	vec4 color_1 = texture(layer_1, TexCoords * uv_scale);
	vec4 color_2 = texture(layer_2, TexCoords * uv_scale);
	vec4 color_3 = texture(layer_3, TexCoords * uv_scale);
	vec4 color_4 = texture(layer_4, TexCoords * uv_scale);
	vec4 color_5 = texture(layer_5, TexCoords * uv_scale);
	vec4 color_6 = texture(layer_6, TexCoords * uv_scale);

	vec4 result = vec4(vec3(0.0), 1.0);

    if (Height < level_2)
    {
        float delta = level_2 - level_1;
        float factor = (Height - level_1) / delta;

        result = mix(color_1, color_2, factor);
    }
    else if (Height < level_3)
    {
        float delta = level_3 - level_2;
        float factor = (Height - level_2) / delta;

        result = mix(color_2, color_3, factor);
    }
    else if (Height < level_4)
    {
        float delta = level_4 - level_3;
        float factor = (Height - level_3) / delta;

        result = mix(color_3, color_4, factor);
    }
    else if (Height < level_5)
    {
        float delta = level_5 - level_4;
        float factor = (Height - level_4) / delta;

        result = mix(color_4, color_5, factor);
    }
    else
    {
        float delta = level_6 - level_5;
        float factor = (Height - level_5) / delta;

        result = mix(color_5, color_6, factor);
    }
		
	FragColor = result;
}
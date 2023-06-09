#version 460 core

out vec4 FragColor;

in VS_OUT
{
    vec3 position;
    vec3 normal;
    vec2 uv;
} fs_in;

uniform sampler2D texture_0;
uniform sampler2D texture_1;

uniform vec4 tint = vec4(0.0, 0.4, 1.0, 0.8);

uniform float uv_scale_0 = 0.3;
uniform float uv_scale_1 = 0.3;

uniform float texture_speed_0 = 0.1;
uniform float texture_speed_1 = 0.08;

uniform float time;

uniform float wobbleX = 1.4;
uniform float wobbleY = 1.7;

void main()
{
    vec4 textureDiffuse0 = texture(texture_0, vec2((fs_in.uv.x * 8) + (time * texture_speed_0), (fs_in.uv.y * 8) + (cos(time) * texture_speed_0)) + cos(time * vec2(wobbleX, wobbleY) + (fs_in.uv.xy * 8) * 10.0) * 0.02 * 1.0);
    vec4 textureDiffuse1 = texture(texture_1, vec2((fs_in.uv.x * 8) + (sin(time) * texture_speed_1), (fs_in.uv.y * 8) + (time * texture_speed_1)) + cos(time * vec2(wobbleX, wobbleY) + (fs_in.uv.xy * 8) * 10.0) * 0.02 * 1.0);

    //1st Layer (Background Colour) - 2nd Layer (Dark Caustics) - 3rd Layer (Light Caustics)
    FragColor = tint + (textureDiffuse0 * uv_scale_0) + (textureDiffuse1 * uv_scale_1);
}
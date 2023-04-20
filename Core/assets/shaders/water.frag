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

uniform vec4 tint = vec4(0.0, 0.4, 1.0, 1.0);

uniform float uv_scale_0 = 1.0;
uniform float uv_scale_1 = 1.0;

uniform float texture_speed_0 = 0.1;
uniform float texture_speed_1 = 0.08;

uniform float time;

uniform float wobbleX = 1.4;
uniform float wobbleY = 1.7;

void main()
{
    vec4 textureDiffuse0 = texture(texture_0, vec2(fs_in.uv.x + (time * texture_speed_0), fs_in.uv.y + (cos(time) * texture_speed_0)) + cos(time * vec2(wobbleX, wobbleY) + fs_in.uv * 10.0) * 0.02 * 1.0);
    vec4 textureDiffuse1 = texture(texture_1, vec2(fs_in.uv.x + (sin(time) * texture_speed_1), fs_in.uv.y + (time * texture_speed_1)) + cos(time * vec2(wobbleX, wobbleY) + fs_in.uv * 10.0) * 0.02 * 1.0);

    //1st Layer (Background Colour) - 2nd Layer (Dark Caustics) - 3rd Layer (Light Caustics)
    FragColor = tint + (textureDiffuse0 * 1.0) + (textureDiffuse1 * 1.0);
}
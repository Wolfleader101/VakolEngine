#version 460 core
layout(quads) in;

uniform sampler2D height_map;

layout (std140, binding = 1) uniform Matrices
{
    mat4 PV_MATRIX;
    mat4 VIEW_MATRIX;
};

uniform mat4 MODEL_MATRIX;

in vec2 TextureCoord[];

out vec2 TexCoord;
out float height;

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec2 t00 = TextureCoord[0];
    vec2 t01 = TextureCoord[1];
    vec2 t10 = TextureCoord[2];
    vec2 t11 = TextureCoord[3];

    vec2 t0 = (t01 - t00) * u + t00;
    vec2 t1 = (t11 - t10) * u + t10;
    vec2 texCoord = (t1 - t0) * v + t0;

    TexCoord = texCoord;

    height = texture(height_map, texCoord).r * 64.0 - 16.0;

    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;

    vec4 up = vec4(0.0, 1.0, 0.0, 0.0);

    vec4 p0 = (p01 - p00) * u + p00;
    vec4 p1 = (p11 - p10) * u + p10;
    vec4 p = (p1 - p0) * v + p0 + up * height;

    gl_Position = PV_MATRIX * MODEL_MATRIX * p;
}
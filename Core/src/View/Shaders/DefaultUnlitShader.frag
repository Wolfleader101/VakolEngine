#version 330

uniform vec4 rgba;

uniform sampler2D inputTexture;

uniform bool enableTexture;

in VertexData
{
    vec4 v_position;
    vec3 v_normal;
    vec2 v_texcoord;
} inData;

out vec4 fragColor;

void main(void)
{
    if(enableTexture == true)
    {
        fragColor = texture(inputTexture, inData.v_texcoord) * rgba;
    }
    else
        fragColor = rgba;
}
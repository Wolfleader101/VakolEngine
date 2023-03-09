#version 330

uniform vec4 rgba; // The tint colour to be applied to the shader

uniform sampler2D inputTexture0; // The texture to be applied to the model

uniform float uvScale0 = 1.0; // The scale of the model's UVs (Useful for resizing the tetxure on the model)

uniform bool enableTexture; // A boolean which lets you switch textures on and off when displaying the model

in VertexData //Data from the Vertex Shader
{
    vec4 v_position;
    vec3 v_normal;
    vec2 v_texcoord;
} inData;

out vec4 fragColor; //The outputed Fragment Colour

void main(void)
{
    if(enableTexture == true) //Checks to see if textures are enabled
    {
        fragColor = texture(inputTexture0, inData.v_texcoord * uvScale0) * rgba;
    }
    else
        fragColor = rgba;
}
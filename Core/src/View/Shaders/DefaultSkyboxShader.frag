#version 460

uniform vec4 rgba = vec4(1.0, 1.0, 1.0, 1.0); // The tint colour to be applied to the shader

uniform samplerCube inputTexture0; // The texture to be applied to the sky

uniform bool enableTexture = true; // A boolean which lets you switch textures on and off when displaying the model

in VertexData //Data from the Vertex Shader
{
    vec2 v_texcoord;
} inData;

out vec4 fragColor; //The outputed Fragment Colour

void main(void)
{
    if(enableTexture == true) //Checks to see if textures are enabled
    {
        fragColor = texture(inputTexture0, inData.v_texcoord) * rgba;
    }
    else
        fragColor = rgba;
}
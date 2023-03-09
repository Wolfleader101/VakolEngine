#version 330

uniform vec4 rgba; // The tint colour to be applied to the shader

uniform sampler2D inputTexture0; // The first texture to be applied to the model
uniform sampler2D inputTexture1; // The second texture to be applied to the model
uniform sampler2D alphaTetxure0; // The alpha texture used to blend the two together

uniform float uvScale0 = 1.0; // The scale of the the UVs for the first texture
uniform float uvScale1 = 1.0; // The scale of the the UVs for the first texture
uniform float alphaUVScale0 = 1.0; // The scale of the the UVs for the first texture

in VertexData //Data from the Vertex Shader
{
    vec4 v_position;
    vec3 v_normal;
    vec2 v_texcoord;
} inData;

out vec4 fragColor; //The outputed Fragment Colour

void main(void)
{
    vec4 texture0, texture1, texture2, finalColour;

    texture0 = texture2D(inputTexture0, inData.v_texcoord * uvScale0) * rgba; //First texture to be blended
    texture1 = texture2D(inputTexture1, inData.v_texcoord * uvScale1) * rgba; //Second texture to be blended
    texture2 = texture2D(alphaTetxure0, inData.v_texcoord * alphaUVScale0); //Alpha Texture

    finalColour = mix(texture0, texture1, texture2); //Blending the textures together using the alpha mask
    
    fragColor = finalColour;
}
#version 460

uniform vec4 rgba = vec4(1.0, 1.0, 1.0, 1.0); // The tint colour to be applied to the shader

uniform sampler2D inputTexture0; // The texture to be applied to the model
uniform sampler2D inputTexture1; // The texture to be applied to the model

uniform float uvScale0 = 1.0; // The scale of the model's UVs (Useful for resizing the tetxure on the model)
uniform float textureAlpha0 = 1.0; // The alpha value of the texture A (0-1)
uniform float textureSpeed0 = 0.1; //Controls the speed at which the first texture moves

uniform float textureAlpha1 = 1.0; // The alpha value of the texture B (0-1)
uniform float uvScale1 = 1.0; // The scale of the model's UVs (Useful for resizing the tetxure on the model)
uniform float textureSpeed1 = 0.08; //Controls the speed at which the second texture moves

uniform float timeVar; //The current time of the scene

uniform float xWobbleIntensity = 1.4; //Controls the intensity of the wobble effect along the X Axis
uniform float yWobbleIntensity = 1.7; //Controls the intensity of the wobble effect along the Y Axis

in VertexData //Data from the Vertex Shader
{
    vec4 v_position;
    vec3 v_normal;
    vec2 v_texcoord;
} inData;

out vec4 fragColor; //The outputed Fragment Colour

void main(void)
{
    vec4 textureDiffuse0 = texture(inputTexture0, 
        vec2(inData.v_texcoord.x + (timeVar * textureSpeed0), 
        inData.v_texcoord.y + (cos(timeVar) * textureSpeed0)) + cos(timeVar * vec2(xWobbleIntensity, yWobbleIntensity) + inData.v_texcoord * 10.0) * 0.02 * uvScale0);
    vec4 textureDiffuse1 = texture(inputTexture1, 
        vec2(inData.v_texcoord.x + (sin(timeVar) * textureSpeed1), 
        inData.v_texcoord.y + (timeVar * textureSpeed1)) + cos(timeVar * vec2(xWobbleIntensity, yWobbleIntensity) + inData.v_texcoord * 10.0) * 0.02 * uvScale1);

    //1st Layer (Background Colour) - 2nd Layer (Dark Caustics) - 3rd Layer (Light Caustics)
    fragColor = rgba - (textureDiffuse0 * textureAlpha0) + (textureDiffuse1 * textureAlpha1);
}
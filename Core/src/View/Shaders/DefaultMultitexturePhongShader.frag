#version 460

uniform vec4 rgba = vec4(1.0, 1.0, 1.0, 1.0); // The tint colour to be applied to the shader

uniform vec3 lightColour = vec3(1.0, 1.0, 1.0); // The colour of the light hitting the model
uniform vec3 lightPos; // The position of the light affecting the shader
uniform vec3 camPos; // The position of the camera

uniform sampler2D inputTexture0; // The texture to be applied to the model
uniform sampler2D inputTexture1; // The second texture to be applied to the model
uniform sampler2D alphaTetxure0; // The alpha texture used to blend the two together

uniform float uvScale0 = 1.0; // The scale of the UVs for the first texture
uniform float uvScale1 = 1.0; // The scale of the UVs for the second texture
uniform float alphaUVScale0 = 1.0; // The scale of the the UVs for the first texture

uniform float ambientMultiplier = 1.0; // The strength of the ambient light colour (0 is none, 1 is full)
uniform float diffuseMultiplier = 0.4; // The strength of the ambient light colour (0 is none, 1 is full)
uniform float specularMultiplier = 3.0; // The strength of the ambient light colour (0 is none, 1 is full)

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
    
    //AMBIENT SECTION//
    vec3 ambientValue = lightColour; //Calculates the ambient value to be applied to the model's Fragment Colour
    
    //DIFFUSE SECTION//
    vec3 normal = normalize(inData.v_normal); //Normalizing the normal (Scaling it down to a value between 0-1)
    float diffuseStrength = max(0.0, dot(lightPos, normal)); //Calculates the strength of the diffuse lighting at each point based on the lights position and the models normals
    vec3 diffuse = diffuseStrength * lightColour; //Calculates the Diffuse Lighting for the model
    
    //SPECULAR SECTION//
    
    vec3 viewSource = normalize(camPos); //Normalizing the camera position (Scaling it down to a value between 0-1)
    vec3 reflectionSource = normalize(reflect(-lightPos, normal)); //Calculating the normalized reflected direction of the negative light position
    
    float specularStrength = max(0.0, dot(viewSource, reflectionSource)); //Calculates the strength of the specular lighting at each point based on the camera's position and the models reflected vectors
    specularStrength =  pow(specularStrength, 32.0); //Calculates the Specular Lighting strength for the model (To the Power of 32 to increase the specularity)
    
    vec3 specular = specularStrength * lightColour; //Calculates the Specular Lighting for the model

    fragColor = finalColour * vec4((ambientValue * ambientMultiplier) + (diffuse * diffuseMultiplier) + (specularStrength * specularMultiplier), 1.0);
}
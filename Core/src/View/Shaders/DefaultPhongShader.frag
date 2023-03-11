#version 460

uniform vec4 rgba = vec4(1.0, 1.0, 1.0, 1.0); // The tint colour to be applied to the shader

uniform vec3 lightColour = vec3(1.0, 1.0, 1.0); // The colour of the light hitting the model
uniform vec3 lightPos; // The position of the light affecting the shader
uniform vec3 camPos; // The position of the camera

uniform sampler2D inputTexture0; // The texture to be applied to the model

uniform float uvScale0 = 1.0; // The scale of the model's UVs (Useful for resizing the tetxure on the model)
uniform float ambientMultiplier = 1.0; // The strength of the ambient light colour (0 is none, 1 is full)
uniform float diffuseMultiplier = 0.4; // The strength of the ambient light colour (0 is none, 1 is full)
uniform float specularMultiplier = 3.0; // The strength of the ambient light colour (0 is none, 1 is full)

uniform bool enableTexture = true; // A boolean which lets you switch textures on and off when displaying the model

in VertexData //Data from the Vertex Shader
{
    vec4 v_position;
    vec3 v_normal;
    vec2 v_texcoord;
} inData;

out vec4 fragColor; //The outputed Fragment Colour

void main(void)
{
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

    if(enableTexture == true) //Checks to see if textures are enabled
    {
        fragColor = texture(inputTexture0, inData.v_texcoord * uvScale0) * (vec4((ambientValue * ambientMultiplier) + (diffuse * diffuseMultiplier) + (specularStrength * specularMultiplier), 1.0) * rgba);
    }
    else
        fragColor = vec4((ambientValue * ambientMultiplier) + (diffuse * diffuseMultiplier) + (specularStrength * specularMultiplier), 1.0) * rgba;
}
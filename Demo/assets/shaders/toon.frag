#version 460 core
out vec4 FragColor;

struct Light
{
	vec3 position;
	vec3 intensity;
	vec3 color;
};

uniform Light light;
const int levels = 3;
const float scaleFactor = 1.0 / 3.0;

uniform bool combineTexAndVert;

uniform sampler2D diffuseTexture;

uniform vec3 Kd;
uniform vec3 Ka;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

void main()
{	
	vec3 lightDir = normalize(light.position - FragPos);
	
	float diff = max(0.0, dot(lightDir, Normal));
	
	vec3 diffuse = Kd * (ceil(diff * levels) * scaleFactor);

	vec3 result = light.intensity * (Ka + diffuse);
	
	FragColor = vec4(result, 1.0);

	if(combineTexAndVert == true)
	{
	    //Combines the texture, toon shading and vertex colours
	    FragColor = texture(diffuseTexture, TexCoord) * FragColor;
	}
	else
	    FragColor = texture(diffuseTexture, TexCoord);
}
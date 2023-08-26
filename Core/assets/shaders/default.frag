#version 460 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

struct Material
{
	vec3 ambient_color;
	vec3 diffuse_color;
	vec3 specular_color;
	vec3 emissive_color;

    sampler2D diffuse_map;
	sampler2D specular_map;
	sampler2D ambient_map;
	sampler2D emissive_map;
	sampler2D height_map;
	sampler2D normal_map;
};

uniform Material material;

uniform vec3 LIGHT_DIRECTION = vec3(0.0, -0.25, 0.0);

vec4 BlinnPhong(const vec3 normal, const vec4 color)
{
    vec4 ambient = 0.2 * color;

    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec4 diffuse = diff * color;

    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec4 specular = vec4(vec3(0.3), 1.0) * spec;

    return ambient + diffuse + specular;
}

void main()
{
    vec4 color = texture(material.diffuse_map, fs_in.TexCoords);

    vec3 normal = texture(material.normal_map, fs_in.TexCoords).rgb;

    if (normal.r >= 0.99 && normal.g >= 0.99 && normal.b >= 0.99)
        normal = fs_in.Normal;
    else
        normal = normalize(normal * 2.0 - 1.0); // this normal is now in tangent space in range [-1, 1]

	FragColor = BlinnPhong(normal, color);
}
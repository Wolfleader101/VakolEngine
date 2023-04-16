#version 460 core
out vec4 FragColor;

struct Material
{
    sampler2D diffuse_map;
    
    float shininess;
};

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform Material material;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{           
    vec3 color = texture(material.diffuse_map, fs_in.TexCoords).rgb;

    // ambient
    vec3 ambient = 0.05 * color;

    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    vec3 normal = normalize(fs_in.Normal);

    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;

    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = vec3(0.3) * spec; // assuming bright white light color
    
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
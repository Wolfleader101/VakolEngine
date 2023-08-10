#pragma once

namespace Vakol::Rendering::Assets
{
    struct MaterialProperties
    {
        glm::vec3 ambient_color = glm::vec3(0.0f);
        glm::vec3 diffuse_color = glm::vec3(0.0f);
        glm::vec3 specular_color = glm::vec3(0.0f);
        glm::vec3 emissive_color = glm::vec3(0.0f);

        float intensity = 0.0f;   // the intensity of the emmisive texture/color
        float shininess = 32.0f;  // the specular exponent (used for Phong shading)
        float opacity = 1.0f;     // the transparency/alpha of the material

        bool wireframe = false;  // determine whether to draw the material in wireframe
    };

    struct Material
    {
        const char* name = nullptr;  // The name of the material

        const char* shader = nullptr;  // The path of the material shader

        MaterialProperties properties;  // the properties that make up a material
    };
}


#pragma once

#include "Texture.hpp"

#include <vector>

namespace Vakol::Rendering::Assets
{
    struct MaterialProperties
    {
        glm::vec3 ambient_color = glm::vec3(0.0f);
        glm::vec3 diffuse_color = glm::vec3(0.0f);
        glm::vec3 specular_color = glm::vec3(0.0f);
        glm::vec3 emissive_color = glm::vec3(0.0f);

        float intensity = 0.0f;  // the intensity of the emmisive texture/color
        float shininess = 32.0f; // the specular exponent (used for Phong shading)
        float opacity = 1.0f;    // the transparency/alpha of the material

        bool wireframe = false; // determine whether to draw the material in wireframe
    };

    struct Material
    {
        std::string name; // The name of the material

        std::string ID = "null";
        std::string shaderID = "null";

        std::vector<Texture> textures;

        MaterialProperties properties; // the properties that make up a material
    };
} // namespace Vakol::Rendering::Assets

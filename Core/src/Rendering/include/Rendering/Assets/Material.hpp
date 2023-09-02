#pragma once

#include "Math/Math.hpp"
#include "Texture.hpp"

#include <vector>

namespace Vakol::Rendering::Assets
{
    /**
     * \brief The various properties of a material.
     */
    struct MaterialProperties
    {
        /**
         * \brief the ambient color
         */
        Math::Vec3 ambient_color = Math::Vec3(0.0f);

        /**
         * \brief the diffuse color
         */
        Math::Vec3 diffuse_color = Math::Vec3(0.0f);

        /**
         * \brief the specular color
         */
        Math::Vec3 specular_color = Math::Vec3(0.0f);

        /**
         * \brief the emission color
         */
        Math::Vec3 emissive_color = Math::Vec3(0.0f);

        float shininess = 32.0f; // the specular exponent (used for Phong shading)
        float shininess_strength = 1.0f;

        float opacity = 1.0f; // the transparency/alpha of the material
    };

    struct Material
    {
        std::string name; /// The name of the material

        std::string ID = "null";
        std::string shaderID = "null";

        std::vector<Texture> textures; /// The textures of a material

        MaterialProperties properties; /// The properties that make up a material
    };
} // namespace Vakol::Rendering::Assets

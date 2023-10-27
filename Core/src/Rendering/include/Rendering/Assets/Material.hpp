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
        Math::Vec4 ambient_color = Math::Vec4(1.0f);

        /**
         * \brief the diffuse color
         */
        Math::Vec4 diffuse_color = Math::Vec4(1.0f);

        /**
         * \brief the specular color
         */
        Math::Vec4 specular_color = Math::Vec4(1.0f);

        /**
         * \brief the emission color
         */
        Math::Vec4 emissive_color = Math::Vec4(1.0f);

        Math::Vec3 light_position = Math::Vec3(5.0f);
        Math::Vec3 light_direction = Math::Vec3(0.0f);

        Math::Vec2 uv_offset = Math::Vec2(0.0f);

        float shininess = 32.0f; // the specular exponent (used for Phong shading)

        float opacity = 1.0f; // the transparency/alpha of the material

        bool use_lighting = true;
        bool use_textures = true;
    };

    struct Material
    {
        std::string name; /// The name of the material

        std::string ID;
        std::string shaderID;

        std::vector<Texture> textures; /// The textures of a material

        MaterialProperties properties; /// The properties that make up a material
    };
} // namespace Vakol::Rendering::Assets

#pragma once

#include "Math/Math.hpp"
#include "Texture.hpp"

#include <vector>

namespace Vakol::Rendering::Assets
{
    enum class LIGHT_TYPE
    {
        DIRECTIONAL_LIGHT,
        SPOT_LIGHT,
        POINT_LIGHT
    };

    /**
     * \brief A struct containing information about a light
     */
    struct Light
    {
        LIGHT_TYPE type = LIGHT_TYPE::DIRECTIONAL_LIGHT;

        const float LIGHT_ATTENUATION_CONSTANT = 1.0f;
        float LIGHT_ATTENUATION_LINEAR = 0.045f;
        float LIGHT_ATTENUATION_QUADRATIC = 0.0075f;

        float LIGHT_ATTENUATION_CUTOFF = Math::Cos(Math::DegToRad(7.5f));
        float LIGHT_ATTENUATION_OUTER_CUTOFF = Math::Cos(Math::DegToRad(12.5f));

        Math::Vec4 LIGHT_DIRECTION = Math::Vec4(0.0f);
    };

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

        /**
         * \brief the offset of uv coordinates
         */
        Math::Vec2 uv_offset = Math::Vec2(0.0f);

        /**
         * \brief the light component
         */
        Light light;

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

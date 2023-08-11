#pragma once

#include <Logger/Logger.hpp>
#include <utility>
#include <vector>

#include "Texture.hpp"
#include "Math/Math.hpp"

namespace Vakol::Assets
{
    /**
     * @brief Struct representing the material specifications.
     */
    struct MaterialSpec
    {
        MaterialSpec() = default;

        Math::Vec3 AMBIENT = Math::Vec3(0.0f);  /**< The ambient color of the material. */
        Math::Vec3 DIFFUSE = Math::Vec3(0.0f);  /**< The diffuse color of the material. */
        Math::Vec3 SPECULAR = Math::Vec3(0.0f); /**< The specular color of the material. */
        Math::Vec3 EMISSIVE = Math::Vec3(0.0f); /**< The emissive color of the material. */
        float SHININESS = 32.0f;                /**< The shininess factor of the material. */

        std::vector<Texture> textures; /**< The textures associated with the material. */
    };

    /**
     * @brief Default material specifications.
     */
    inline MaterialSpec DEFAULT = {Math::Vec3(1.0f), Math::Vec3(0.6f), Math::Vec3(0.2f), Math::Vec3(0.0f), 32.0f, {}};

    /**
     * @brief Class representing a material.
     */
    class Material
    {
      public:
        /**
         * @brief Constructs a Material object with the given material specifications.
         *
         * @param spec The material specifications.
         */
        explicit Material(MaterialSpec&& spec) : m_spec(std::move(spec))
        {
            this->m_textures = std::move(m_spec.textures);
        }

        /**
         * @brief Adds a texture to the material.
         *
         * @param texture The texture to add.
         */
        void AddTexture(Texture& texture)
        {
            this->m_textures.push_back(std::move(texture));
        }

        /**
         * @brief Gets the ID of the texture at the specified index.
         *
         * @param index The index of the texture.
         * @return The ID of the texture.
         */
        [[nodiscard]] unsigned int GetTexture(const int index) const
        {
            if (GetTextureCount() <= index)
            {
                VK_CRITICAL("Material::GetTexture() - Texture index out of bounds.");
                return 0;
            }

            return this->m_textures.at(index).GetID();
        }

        /**
         * @brief Gets the vector of textures associated with the material.
         *
         * @return The vector of textures.
         */
        [[nodiscard]] const std::vector<Texture>& GetTextures() const
        {
            return this->m_textures;
        }

        /**
         * @brief Gets the number of textures associated with the material.
         *
         * @return The number of textures.
         */
        [[nodiscard]] int GetTextureCount() const
        {
            return static_cast<int>(this->m_textures.size());
        }

        /**
         * @brief Sets the ambient color of the material.
         *
         * @param r The red component of the color.
         * @param g The green component of the color.
         * @param b The blue component of the color.
         */
        void SetAmbientColor(const float r, const float g, const float b)
        {
            this->m_spec.AMBIENT = Math::Vec3(r, g, b);
        }

        /**
         * @brief Sets the diffuse color of the material.
         *
         * @param r The red component of the color.
         * @param g The green component of the color.
         * @param b The blue component of the color.
         */
        void SetDiffuseColor(const float r, const float g, const float b)
        {
            this->m_spec.DIFFUSE = Math::Vec3(r, g, b);
        }

        /**
         * @brief Sets the specular color of the material.
         *
         * @param r The red component of the color.
         * @param g The green component of the color.
         * @param b The blue component of the color.
         */
        void SetSpecularColor(const float r, const float g, const float b)
        {
            this->m_spec.SPECULAR = Math::Vec3(r, g, b);
        }

        /**
         * @brief Sets the shininess factor of the material.
         *
         * @param shine The shininess factor.
         */
        void SetShininess(const float shine)
        {
            this->m_spec.SHININESS = shine;
        }

        /**
         * @brief Gets the ambient color of the material.
         *
         * @return The ambient color.
         */
        [[nodiscard]] Math::Vec3 GetAmbientColor() const
        {
            return this->m_spec.AMBIENT;
        }

        /**
         * @brief Gets the diffuse color of the material.
         *
         * @return The diffuse color.
         */
        [[nodiscard]] Math::Vec3 GetDiffuseColor() const
        {
            return this->m_spec.DIFFUSE;
        }

        /**
         * @brief Gets the specular color of the material.
         *
         * @return The specular color.
         */
        [[nodiscard]] Math::Vec3 GetSpecularColor() const
        {
            return this->m_spec.SPECULAR;
        }

        /**
         * @brief Gets the shininess factor of the material.
         *
         * @return The shininess factor.
         */
        [[nodiscard]] float GetShininess() const
        {
            return this->m_spec.SHININESS;
        }

      private:
        MaterialSpec m_spec;             /**< The material specifications. */
        std::vector<Texture> m_textures; /**< The textures associated with the material. */
    };
} // namespace Vakol::Assets
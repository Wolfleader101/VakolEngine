#pragma once

#include <glm/glm.hpp>
#include <utility>
#include <vector>

#include "Texture.hpp"

#include <Controller/Logger.hpp>

namespace Vakol::Model::Assets 
{
    struct MaterialSpec 
    {
        MaterialSpec() = default;

        glm::vec3 AMBIENT = glm::vec3(0.0f);
        glm::vec3 DIFFUSE = glm::vec3(0.0f);
        glm::vec3 SPECULAR = glm::vec3(0.0f);
        glm::vec3 EMISSIVE = glm::vec3(0.0f);
        float SHININESS = 32.0f;

        std::vector<unsigned int> textures;
    };

    inline MaterialSpec DEFAULT = {glm::vec3(1.0f), glm::vec3(0.6f), glm::vec3(0.2f), glm::vec3(0.0f), 32.0f, {}};

    class Material 
    {
    public:
        explicit Material(MaterialSpec&& spec) : m_spec(std::move(spec)) { this->m_textures = std::move(m_spec.textures); }

        void AddTexture(Texture& texture) { this->m_textures.push_back(std::move(texture)); }

        [[nodiscard]] const Texture& GetTexture(const int index) const 
        { 
            VK_ASSERT(GetTextureCount() > index, "\n\nTexture index out of bounds.");

            return this->m_textures.at(index);
        }

        [[nodiscard]] const std::vector<Texture>& GetTextures() const { return this->m_textures; }
        [[nodiscard]] int GetTextureCount() const { return static_cast<int>(this->m_textures.size()); }

        void SetAmbientColor(const float r, const float g, const float b)  { this->m_spec.AMBIENT = glm::vec3(r,g,b); }
        void SetDiffuseColor(const float r, const float g, const float b)  { this->m_spec.DIFFUSE = glm::vec3(r,g,b); }
        void SetSpecularColor(const float r, const float g, const float b)  { this->m_spec.SPECULAR = glm::vec3(r,g,b); }
        void SetShininess(const float shine)  { this->m_spec.SHININESS = shine; }

        [[nodiscard]] glm::vec3 GetAmbientColor() const { return this->m_spec.AMBIENT; }
        [[nodiscard]] glm::vec3 GetDiffuseColor() const { return this->m_spec.DIFFUSE; }
        [[nodiscard]] glm::vec3 GetSpecularColor() const { return this->m_spec.SPECULAR; }
        [[nodiscard]] float GetShininess() const { return this->m_spec.SHININESS; }

    private:
        MaterialSpec m_spec;
        std::vector<Texture> m_textures;
    };
}
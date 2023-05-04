#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Shader.hpp"
#include "Texture.hpp"

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

        std::vector<Texture> textures;
    };

    const MaterialSpec DEFAULT = {glm::vec3(1.0f), glm::vec3(0.6f), glm::vec3(0.2f), glm::vec3(0.0f), 32.0f };

    class Material 
    {
    public:
        Material(const MaterialSpec& spec) : m_spec(std::move(spec)) {};

        void AddTexture(const Texture& texture) { this->m_spec.textures.push_back(texture); }

        const Texture GetTexture(const int index) const { return this->m_spec.textures.at(index); }
        const std::vector<Texture> GetTextures() const  { return this->m_spec.textures; }
        const int GetTextureCount() const { return static_cast<int>(this->m_spec.textures.size()); }

        void SetAmbientColor(const float r, const float g, const float b)  { this->m_spec.AMBIENT = glm::vec3(r,g,b); }
        void SetDiffuseColor(const float r, const float g, const float b)  { this->m_spec.DIFFUSE = glm::vec3(r,g,b); }
        void SetSpecularColor(const float r, const float g, const float b)  { this->m_spec.SPECULAR = glm::vec3(r,g,b); }
        void SetShininess(const float shine)  { this->m_spec.SHININESS = shine; }

        const glm::vec3 GetAmbientColor() const  { return this->m_spec.AMBIENT; }
        const glm::vec3 GetDiffuseColor() const  { return this->m_spec.DIFFUSE; }
        const glm::vec3 GetSpecularColor() const  { return this->m_spec.SPECULAR; }
        const float GetShininess() const  { return this->m_spec.SHININESS; }

    private:
        MaterialSpec m_spec;
    };
}  // namespace Vakol::Model::Assets
#pragma once

#include <Model/Assets/Material.hpp>
#include <vector>

#include "GLTexture.hpp"

using namespace Vakol::Model::Assets;
namespace Vakol::Model {

    class GLMaterial : public Material 
    {
    public:
        GLMaterial(const MaterialSpec& spec) : Material(spec){};

        const int GetTextureCount() const override {return static_cast<int>(this->m_spec.textures.size()); }

        std::vector<Texture> textures() override { return this->m_spec.textures; }

        void SetAmbientColor(const float r, const float g, const float b) override { this->m_spec.AMBIENT = glm::vec3(r,g,b); }
        void SetDiffuseColor(const float r, const float g, const float b) override { this->m_spec.DIFFUSE = glm::vec3(r,g,b); }
        void SetSpecularColor(const float r, const float g, const float b) override { this->m_spec.SPECULAR = glm::vec3(r,g,b); }
        void SetShininess(const float shine) override { this->m_spec.SHININESS = shine; }

        const glm::vec3 GetAmbientColor() const override { return this->m_spec.AMBIENT; }
        const glm::vec3 GetDiffuseColor() const override { return this->m_spec.DIFFUSE; }
        const glm::vec3 GetSpecularColor() const override { return this->m_spec.SPECULAR; }
        const float GetShininess() const override { return this->m_spec.SHININESS; }
    };
}  // namespace Vakol::Model

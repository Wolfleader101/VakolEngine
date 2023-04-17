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

        const glm::vec3 ambient() const override { return this->m_spec.AMBIENT; }
        const glm::vec3 diffuse() const override { return this->m_spec.DIFFUSE; }
        const glm::vec3 specular() const override { return this->m_spec.SPECULAR; }
        const float shininess() const override { return this->m_spec.SHININESS; }
    };
}  // namespace Vakol::Model

#pragma once

#include <Model/Assets/Material.hpp>
#include <vector>

#include "GLShader.hpp"
#include "GLTexture.hpp"

using namespace Vakol::Model::Assets;
namespace Vakol::Model {

    class GLMaterial : public Material 
    {
    public:
        GLMaterial(const MaterialSpec& spec) : Material(spec){};

        void SetShader(const std::string& path) override;
        void AddUniform(const int size, const int binding) override;
        void SetUniformData(const int index, const int offset, const int size, const void* data) const override;

        void Bind() const override;
        void Unbind() const override;

        const unsigned int GetID() const override { return this->m_shader->GetID(); };
        const int GetTextureCount() const override {return static_cast<int>(this->m_spec.textures.size()); }

        std::vector<Texture> textures() override { return this->m_spec.textures; }

        const float shininess() const override { return this->m_spec.SHININESS; }

    public:
        void SetBool(const std::string& name, const bool value) const override;
        void SetInt(const std::string& name, const int value) const override;
        void SetFloat(const std::string& name, const float value) const override;

        void SetVec2(const std::string& name, const glm::vec2& value) const override;
        void SetVec2(const std::string& name, const float x, const float y) const override;

        void SetVec3(const std::string& name, const glm::vec3& value) const override;
        void SetVec3(const std::string& name, const float x, const float y, const float z) const override;

        void SetVec4(const std::string& name, const glm::vec4& value) const override;

        void SetMat3(const std::string& name, const glm::mat3& value) const override;
        void SetMat4(const std::string& name, const glm::mat4& value) const override;
    };
}  // namespace Vakol::Model

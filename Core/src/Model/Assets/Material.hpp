#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Shader.hpp"
#include "Texture.hpp"

#include <Model/UniformBuffer.hpp>

namespace Vakol::Model::Assets {
    struct MaterialSpec {
        MaterialSpec() = default;

        glm::vec3 AMBIENT = glm::vec3(1.0f);
        glm::vec3 DIFFUSE = glm::vec3(1.0f);
        glm::vec3 SPECULAR = glm::vec3(0.5f);
        glm::vec3 EMISSIVE = glm::vec3(0.0f);
        float SHININESS = 32.0f;

        std::vector<Texture> textures;
    };

    class Material {
       public:
        Material(const MaterialSpec& spec) : m_spec(spec){};

        virtual void SetShader(const std::string& path) = 0;

        virtual void AddUniform(const int size, const int binding) = 0;
        virtual void SetUniformData(const int index, const int offset, const int size, const void* data) const = 0;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual const unsigned int GetID() const = 0;
        virtual const int GetTextureCount() const = 0;

        virtual std::vector<Texture> textures() = 0;

        virtual const float shininess() const = 0;

       public:
        virtual void SetBool(const std::string& name, const bool value) const = 0;
        virtual void SetInt(const std::string& name, const int value) const = 0;
        virtual void SetFloat(const std::string& name, const float value) const = 0;

        virtual void SetVec2(const std::string& name, const glm::vec2& value) const = 0;
        virtual void SetVec2(const std::string& name, const float x, const float y) const = 0;

        virtual void SetVec3(const std::string& name, const glm::vec3& value) const = 0;
        virtual void SetVec3(const std::string& name, const float x, const float y, const float z) const = 0;

        virtual void SetVec4(const std::string& name, const glm::vec4& value) const = 0;

        virtual void SetMat3(const std::string& name, const glm::mat3& value) const = 0;
        virtual void SetMat4(const std::string& name, const glm::mat4& value) const = 0;

    protected:
        std::shared_ptr<Shader> m_shader = nullptr;
        std::vector<std::shared_ptr<UniformBuffer>> m_uniforms;
        MaterialSpec m_spec;
    };
}  // namespace Vakol::Model::Assets
#pragma once

#include <Controller/AssetLoader/ShaderLoader.hpp>
#include <glm/glm.hpp>
#include <string>

using Vakol::Controller::LoadShader;

namespace Vakol::Model {
    class Shader {
       public:
        Shader() = default;
        explicit Shader(const std::string& path) : m_ID(LoadShader(path)) {}

        ~Shader();

        void Bind() const;
        void Unbind() const;

        [[nodiscard]] unsigned int GetID() const { return m_ID; }

       public:
        void SetBool(const char* name, bool value) const;

        void SetInt(const char* name, int value) const;

        void SetFloat(const char* name, float value) const;

        void SetVec2(const char* name, const glm::vec2& value) const;
        void SetVec2(const char* name, float x, float y) const;

        void SetVec3(const char* name, const glm::vec3& value) const;
        void SetVec3(const char* name, float x, float y, float z) const;

        void SetVec4(const char* name, const glm::vec4& value) const;
        void SetVec4(const char* name, float x, float y, float z, float w) const;

        void SetMat3(const char* name, const glm::mat3& value) const;
        void SetMat4(const char* name, const glm::mat4& value) const;
        void SetMat4v(const char* name, int count, const void* data) const;

       private:
        unsigned int m_ID = 0;
    };
}  // namespace Vakol::Model
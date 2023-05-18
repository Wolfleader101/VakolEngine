#pragma once

#include <string>

#include <Controller/AssetLoader/ShaderLoader.hpp>

#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

using Vakol::Controller::LoadShader;

namespace Vakol::Model 
{
    class Shader 
    {
    public:
        Shader(const std::string& path) : m_ID(LoadShader(path)) {};
        ~Shader();

        void Bind() const;
        void Unbind() const;

        [[nodiscard]] unsigned int GetID() const { return m_ID; }

    public:
        void SetBool(const char* name, const bool value) const;

        void SetInt(const char* name, const int value) const;

        void SetFloat(const char* name, const float value) const;

        void SetVec2(const char* name, const glm::vec2& value) const;
        void SetVec2(const char* name, const float x, const float y) const;

        void SetVec3(const char* name, const glm::vec3& value) const;
        void SetVec3(const char* name, const float x, const float y, const float z) const;

        void SetVec4(const char* name, const glm::vec4& value) const ;
        void SetVec4(const char* name, const float x, const float y, const float z, const float w) const;

        void SetMat3(const char* name, const glm::mat3& value) const;
        void SetMat4(const char* name, const glm::mat4& value) const;
        void SetMat4v(const char* name, const int count, const void* data) const;

    private:
        unsigned int m_ID = 0;
    };
}
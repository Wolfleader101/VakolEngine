#pragma once

#include <Controller/AssetLoader/ShaderLoader.hpp>
#include <glm/glm.hpp>
#include <string>

using Vakol::Controller::LoadShader;

namespace Vakol::Model
{
    /**
     * @brief Class representing a Shader.
     */
    class Shader
    {
      public:
        /**
         * @brief Default constructor for Shader class.
         */
        Shader() = default;

        /**
         * @brief Constructor for Shader class.
         * @param path Path to the shader file.
         */
        explicit Shader(const std::string& path) : m_ID(LoadShader(path))
        {
        }

        /**
         * @brief Destructor for Shader class.
         */
        ~Shader();

        /**
         * @brief Function to bind the shader.
         */
        void Bind() const;

        /**
         * @brief Function to unbind the shader.
         */
        void Unbind() const;

        /**
         * @brief Function to get the shader's ID.
         * @return The shader's ID.
         */
        unsigned int GetID() const
        {
            return m_ID;
        };

      public:
        /**
         * @brief Function to set a boolean uniform in the shader.
         * @param name Name of the uniform.
         * @param value Boolean value to set.
         */
        void SetBool(const char* name, bool value) const;

        /**
         * @brief Function to set an integer uniform in the shader.
         * @param name Name of the uniform.
         * @param value Integer value to set.
         */
        void SetInt(const char* name, int value) const;

        /**
         * @brief Function to set a float uniform in the shader.
         * @param name Name of the uniform.
         * @param value Float value to set.
         */
        void SetFloat(const char* name, float value) const;

        /**
         * @brief Function to set a vec2 uniform in the shader.
         * @param name Name of the uniform.
         * @param value glm::vec2 value to set.
         */
        void SetVec2(const char* name, const glm::vec2& value) const;
        void SetVec2(const char* name, float x, float y) const;

        /**
         * @brief Function to set a vec3 uniform in the shader.
         * @param name Name of the uniform.
         * @param value glm::vec3 value to set.
         */
        void SetVec3(const char* name, const glm::vec3& value) const;
        void SetVec3(const char* name, float x, float y, float z) const;

        /**
         * @brief Function to set a vec4 uniform in the shader.
         * @param name Name of the uniform.
         * @param value glm::vec4 value to set.
         */
        void SetVec4(const char* name, const glm::vec4& value) const;
        void SetVec4(const char* name, float x, float y, float z, float w) const;

        /**
         * @brief Function to set a mat3 uniform in the shader.
         * @param name Name of the uniform.
         * @param value glm::mat3 value to set.
         */
        void SetMat3(const char* name, const glm::mat3& value) const;

        /**
         * @brief Function to set a mat4 uniform in the shader.
         * @param name Name of the uniform.
         * @param value glm::mat4 value to set.
         */
        void SetMat4(const char* name, const glm::mat4& value) const;
        void SetMat4v(const char* name, int count, const void* data) const;

      private:
        unsigned int m_ID = 0; ///< ID of the Shader.
    };
} // namespace Vakol::Model
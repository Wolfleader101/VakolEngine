#pragma once

// C++ Standard Libraries
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

// Imported Libraries

// Vakol Libraries
#include "AssetLoader/FileLoader.hpp"

#include "Math/Math.hpp"

#include "Rendering/Platform/OpenGL/Shader.hpp"
#include "Rendering/Assets/Shader.hpp"
#include "Rendering/RenderData.hpp"

#include "Logger/Logger.hpp"

namespace Vakol
{
    class ShaderProcessing
    {
      public:
        /**
         * @brief Add a shader to the shader map
         * @param shaderID The ID of the shader
         * @param shader The shader to add
         */
        void AddShader(const std::string& shaderID, unsigned int shader);

        /**
         * @brief Get a shader from the shader map
         * @param shaderID The ID of the shader
         * @return The shader as an unsigned int
         */
        unsigned int GetShader(const std::string& shaderID);

        /**
         * @brief Set a boolean uniform in a shader
         * @param shader The shader to set the uniform in
         * @param name The name of the uniform
         * @param value The value of the uniform
         * @param binding Whether the uniform is a binding or not
         */
        void SetBool(unsigned int shader, const char* name, bool value, bool binding = true);

        /**
         * @brief Set an integer uniform in a shader
         * @param shader The shader to set the uniform in
         * @param name The name of the uniform
         * @param value The value of the uniform
         * @param binding Whether the uniform is a binding or not
         */
        void SetInt(unsigned int shader, const char* name, int value, bool binding = true);

        /**
         * @brief Set an unsigned integer uniform in a shader
         * @param shader The shader to set the uniform in
         * @param name The name of the uniform
         * @param value The value of the uniform
         * @param binding Whether the uniform is a binding or not
         */
        void SetUInt(unsigned int shader, const char* name, unsigned int value, bool binding = true);

        /**
         * @brief Set a float uniform in a shader
         * @param shader The shader to set the uniform in
         * @param name The name of the uniform
         * @param value The value of the uniform
         * @param binding Whether the uniform is a binding or not
         */
        void SetFloat(unsigned int shader, const char* name, float value, bool binding = true);
        /**
         * @brief Set a Vector 2 uniform in a shader
         * @param shader The shader to set the uniform in
         * @param name The name of the uniform
         * @param value The value of the uniform
         * @param binding Whether the uniform is a binding or not
         */
        void SetVec2(unsigned int shader, const char* name, const Math::Vec2& value, bool binding = true);
        /**
         * @brief Set a Vector 3 uniform in a shader
         * @param shader The shader to set the uniform in
         * @param name The name of the uniform
         * @param value The value of the uniform
         * @param binding Whether the uniform is a binding or not
         */
        void SetVec3(unsigned int shader, const char* name, const Math::Vec3& value, bool binding = true);
        /**
         * @brief Set a Vector 4 uniform in a shader
         * @param shader The shader to set the uniform in
         * @param name The name of the uniform
         * @param value The value of the uniform
         * @param binding Whether the uniform is a binding or not
         */
        void SetVec4(unsigned int shader, const char* name, const Math::Vec4& value, bool binding = true);
        /**
         * @brief Set a Matrix 3 uniform in a shader
         * @param shader The shader to set the uniform in
         * @param name The name of the uniform
         * @param transpose Whether the matrix should be transposed or not
         * @param value The value of the uniform
         * @param binding Whether the uniform is a binding or not
         */
        void SetMat3(unsigned int shader, const char* name, bool transpose, const Math::Mat3& value,
                     bool binding = true);
        /**
         * @brief Set a Matrix 4 uniform in a shader
         * @param shader The shader to set the uniform in
         * @param name The name of the uniform
         * @param transpose Whether the matrix should be transposed or not
         * @param value The value of the uniform
         * @param binding Whether the uniform is a binding or not
         */
        void SetMat4(unsigned int shader, const char* name, bool transpose, const Math::Mat4& value,
                     bool binding = true);

        /**
         * @brief Tells if the shader map is empty
         * @return True if the shader map is empty, false otherwise
         */
        bool IsEmpty() const;

        /**
         * @brief Import a shader program
         * @param path The path of the shader program
         * @param success Whether the import was successful or not
         * @return The imported shader program
         */
        Rendering::Assets::Shader ImportShader(const std::string& path, bool& success);

        /**
         * @brief A getter for the shader source code
         * @param paths The paths of the shader source code
         * @param shader The shader to get the source code for
         * @return Whether the source code was successfully retrieved or not
         */
        bool GetShaderSources(std::vector<std::string>&& paths, Rendering::Assets::Shader& shader);
      private:
        /**
         * @brief The shader map
         */
        std::unordered_map<std::string, unsigned int> m_shaders;
        /**
         * @brief The uniform map
         */
        std::map<unsigned int, std::unordered_map<std::string, Rendering::Uniform>> m_uniforms;

        /**
         * @brief Get the shader uniforms
         * @param shader The shader to get the uniforms for
         */
        void GetShaderUniforms(unsigned int shader);

        /**
         * @brief Get a uniform from a shader
         * @param shader The shader to get the uniform from
         * @param name The name of the uniform
         * @return The uniform
         */
        Rendering::Uniform& GetUniform(unsigned int shader, const char* name);

        /**
         * @brief Check if a uniform exists in a shader
         * @param shader The shader to check
         * @param name The name of the uniform
         * @return Whether the uniform exists or not
         */
        bool UniformExists(unsigned int shader, const char* name);
    };
} // namespace Vakol

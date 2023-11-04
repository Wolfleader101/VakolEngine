#pragma once

#include <map>
#include <string>
#include <unordered_map>

#include "Math/Math.hpp"

#include "Rendering/RenderData.hpp"
#include "Rendering/Platform/OpenGL/Shader.hpp"

#include "Logger/Logger.hpp"

namespace Vakol
{
    class ShaderLibrary
    {
      public:
        /**
         * \brief
         * \param shaderID
         * \param shader
         */
        void AddShader(const std::string& shaderID, unsigned int shader);

        /**
         * \brief
         * \param shaderID
         * \return
         */
        unsigned int GetShader(const std::string& shaderID);

        /**
         * \brief
         * \param shader
         * \param name
         * \param value
         * \param binding
         */
        void SetBool(unsigned int shader, const char* name, bool value, bool binding = true);

        /**
         * \brief
         * \param shader
         * \param name
         * \param value
         * \param binding
         */
        void SetInt(unsigned int shader, const char* name, int value, bool binding = true);

        /**
         * \brief
         * \param shader
         * \param name
         * \param value
         * \param binding
         */
        void SetUInt(unsigned int shader, const char* name, unsigned int value, bool binding = true);

        /**
         * \brief
         * \param shader
         * \param name
         * \param value
         * \param binding
         */
        void SetFloat(unsigned int shader, const char* name, float value, bool binding = true);
        /**
         * \brief
         * \param shader
         * \param name
         * \param value
         * \param binding
         */
        void SetVec2(unsigned int shader, const char* name, const Math::Vec2& value, bool binding = true);
        /**
         * \brief
         * \param shader
         * \param name
         * \param value
         * \param binding
         */
        void SetVec3(unsigned int shader, const char* name, const Math::Vec3& value, bool binding = true);
        /**
         * \brief
         * \param shader
         * \param name
         * \param value
         * \param binding
         */
        void SetVec4(unsigned int shader, const char* name, const Math::Vec4& value, bool binding = true);
        /**
         * \brief
         * \param shader
         * \param name
         * \param transpose
         * \param value
         * \param binding
         */
        void SetMat3(unsigned int shader, const char* name, bool transpose, const Math::Mat3& value,
                     bool binding = true);
        /**
         * \brief
         * \param shader
         * \param name
         * \param transpose
         * \param value
         * \param binding
         */
        void SetMat4(unsigned int shader, const char* name, bool transpose, const Math::Mat4& value,
                     bool binding = true);

        /**
         * \brief function that returns if a shader map is empty
         * \return whether the shader map is empty
         */
        bool IsEmpty() const;

      private:
        /**
         * \brief
         */
        std::unordered_map<std::string, unsigned int> m_shaders;
        /**
         * \brief
         */
        std::map<unsigned int, std::unordered_map<std::string, Rendering::Uniform>> m_uniforms;

        /**
         * \brief
         * \param shader
         */
        void GetShaderUniforms(unsigned int shader);
        /**
         * \brief
         * \param shader
         * \param name
         * \return
         */
        Rendering::Uniform& GetUniform(unsigned int shader, const char* name);

        /**
         * \brief
         * \param shader
         * \param name
         * \return
         */
        bool UniformExists(unsigned int shader, const char* name);
    };
} // namespace Vakol
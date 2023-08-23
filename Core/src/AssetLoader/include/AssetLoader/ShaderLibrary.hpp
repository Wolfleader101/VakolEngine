#pragma once

#include <map>
#include <string>
#include <unordered_map>

#include "Math/Math.hpp"

namespace Vakol::Rendering
{
    struct Uniform;
} // namespace Vakol::Rendering

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
         */
        void SetBool(unsigned int shader, const char* name, bool value);

        /**
         * \brief
         * \param shader
         * \param name
         * \param value
         */
        void SetInt(unsigned int shader, const char* name, int value);

        /**
         * \brief
         * \param shader
         * \param name
         * \param value
         */
        void SetFloat(unsigned int shader, const char* name, float value);
        /**
         * \brief
         * \param shader
         * \param name
         * \param value
         */
        void SetVec2(unsigned int shader, const char* name, const Math::Vec2& value);
        /**
         * \brief
         * \param shader
         * \param name
         * \param value
         */
        void SetVec3(unsigned int shader, const char* name, const Math::Vec3& value);
        /**
         * \brief
         * \param shader
         * \param name
         * \param value
         */
        void SetVec4(unsigned int shader, const char* name, const Math::Vec4& value);
        /**
         * \brief
         * \param shader
         * \param name
         * \param transpose
         * \param value
         */
        void SetMat3(unsigned int shader, const char* name, bool transpose, const Math::Mat3& value);
        /**
         * \brief
         * \param shader
         * \param name
         * \param transpose
         * \param value
         */
        void SetMat4(unsigned int shader, const char* name, bool transpose, const Math::Mat4& value);

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
#pragma once

#include <string>
#include <unordered_map>

struct Uniform;

namespace Vakol::Rendering::OpenGL
{
    /**
     * \brief
     * \param vertSrc
     * \param geomSrc
     * \param tscSrc
     * \param tseSrc
     * \param fragSrc
     * \return
     */
    unsigned int GenerateShaderProgram(const std::string& vertSrc, const std::string& geomSrc,
                                       const std::string& tscSrc, const std::string& tseSrc,
                                       const std::string& fragSrc);

    /**
     * \brief
     * \param program
     */
    void BindShaderProgram(unsigned int program);
    /**
     * \brief
     */
    void UnbindShaderProgram();

    /**
     * \brief
     * \param shader
     * \param uniforms
     */
    void GetUniforms(unsigned int shader, std::unordered_map<std::string, Uniform>& uniforms);

    /**
     * \brief
     * \param location
     * \param value
     */
    void SetBool(int location, bool value);
    /**
     * \brief
     * \param location
     * \param value
     */
    void SetFloat(int location, float value);
    /**
     * \brief
     * \param location
     * \param value
     */
    void SetInt(int location, int value);

    /**
     * \brief
     * \param location
     * \param count
     * \param value
     */
    void SetVec2(int location, int count, const float* value);
    /**
     * \brief
     * \param location
     * \param count
     * \param value
     */
    void SetVec3(int location, int count, const float* value);
    /**
     * \brief
     * \param location
     * \param count
     * \param value
     */
    void SetVec4(int location, int count, const float* value);

    /**
     * \brief
     * \param location
     * \param count
     * \param name
     * \param transpose
     * \param value
     */
    void SetMat3(int location, int count, const char* name, bool transpose, const float* value);
    /**
     * \brief
     * \param location
     * \param count
     * \param name
     * \param transpose
     * \param value
     */
    void SetMat4(int location, int count, const char* name, bool transpose, const float* value);
} // namespace Vakol::Rendering::OpenGL
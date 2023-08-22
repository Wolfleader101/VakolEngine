#include "AssetLoader/AssetLoader.hpp"

#include "Logger/Logger.hpp"

#include <algorithm>

namespace Vakol
{
    std::string AssetLoader::model_path = "assets/models/";
    std::string AssetLoader::texture_path = "assets/textures/";
    std::string AssetLoader::shader_path = "assets/shaders/";

    ModelLibrary AssetLoader::m_modelLibrary;
    ShaderLibrary AssetLoader::m_shaderLibrary;
    TextureLibrary AssetLoader::m_textureLibrary;

    void AssetLoader::AddShader(const std::string& shaderID, const unsigned int shader)
    {
        m_shaderLibrary.AddShader(shaderID, shader);
    }

    void AssetLoader::SetBool(const unsigned int shader, const char* name, const bool value)
    {
        m_shaderLibrary.SetBool(shader, name, value);
    }

    void AssetLoader::SetInt(const unsigned int shader, const char* name, const int value)
    {
        m_shaderLibrary.SetInt(shader, name, value);
    }

    void AssetLoader::SetFloat(const unsigned shader, const char* name, const float value)
    {
        m_shaderLibrary.SetFloat(shader, name, value);
    }

    void AssetLoader::SetVec2(const unsigned int shader, const char* name, const Math::Vec2& value)
    {
        m_shaderLibrary.SetVec2(shader, name, value);
    }

    void AssetLoader::SetVec3(const unsigned int shader, const char* name, const Math::Vec3& value)
    {
        m_shaderLibrary.SetVec3(shader, name, value);
    }

    void AssetLoader::SetVec4(const unsigned int shader, const char* name, const Math::Vec4& value)
    {
        m_shaderLibrary.SetVec4(shader, name, value);
    }

    void AssetLoader::SetMat3(const unsigned int shader, const char* name, const bool transpose,
                              const Math::Mat3& value)
    {
        m_shaderLibrary.SetMat3(shader, name, transpose, value);
    }

    void AssetLoader::SetMat4(const unsigned int shader, const char* name, const bool transpose,
                              const Math::Mat4& value)
    {
        m_shaderLibrary.SetMat4(shader, name, transpose, value);
    }

    Rendering::Assets::Model& AssetLoader::FindModel(const std::string& path)
    {
        return m_modelLibrary.FindModel(path);
    }

    Rendering::Assets::Model& AssetLoader::GetModel(const std::string& path, const float scale)
    {
        return m_modelLibrary.GetModel(path, scale);
    }

    const std::vector<Rendering::Assets::Mesh>& AssetLoader::GetMeshes(const std::string& modelID)
    {
        return m_modelLibrary.FindModel(modelID).meshes;
    }

    unsigned int AssetLoader::GetShader(const std::string& shaderID)
    {
        return m_shaderLibrary.GetShader(shaderID);
    }

    Rendering::Assets::Texture& AssetLoader::GetTexture(const std::string& path, const unsigned int type,
                                                        const int levels)
    {
        return m_textureLibrary.GetTexture(path, type, levels);
    }

    Rendering::Assets::Texture& AssetLoader::GetTexture(const std::string& path, const unsigned int type,
                                                        const int size, const void* data, const int levels)
    {
        return m_textureLibrary.GetTexture(path, type, size, data, levels);
    }

    void AssetLoader::ReplaceTexture(const std::string& modelID, const std::string& srcPath, const unsigned int srcType,
                                     const std::string& dstPath, const unsigned int dstType)
    {
        const auto& model = FindModel(modelID);

        for (const auto& mesh : model.meshes)
        {
            auto& textures = mesh.material->textures;

            std::replace_if(
                textures.begin(), textures.end(),
                [&](const Rendering::Assets::Texture& texture) {
                    return texture.path == srcPath && texture.type == srcType;
                },
                GetTexture(dstPath, dstType));
        }
    }

} // namespace Vakol

#include "AssetLoader/AssetLoader.hpp"

#include "Logger/Logger.hpp"

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

    void AssetLoader::SetMat4(const unsigned int shader, const char* name, const bool transpose,
                              const Math::Mat4& value)
    {
        m_shaderLibrary.SetMat4(shader, name, transpose, value);
    }

    Rendering::Assets::Model& AssetLoader::GetModel(const std::string& path, const float scale)
    {
        return m_modelLibrary.GetModel(path, scale);
    }

    const std::vector<Rendering::Assets::Mesh>& AssetLoader::GetMeshes(const std::string& modelID)
    {
        return m_modelLibrary.GetModel(modelID).meshes;
    }

    unsigned int AssetLoader::GetShader(const std::string& shaderID)
    {
        return m_shaderLibrary.GetShader(shaderID);
    }

    Rendering::Assets::Texture& AssetLoader::GetTexture(const std::string& path, const unsigned int type)
    {
        return m_textureLibrary.GetTexture(path, type);
    }

    Rendering::Assets::Texture& AssetLoader::GetTexture(const std::string& path, const unsigned int type,
                                                        const int size, const void* data)
    {
        return m_textureLibrary.GetTexture(path, type, size, data);
    }
} // namespace Vakol

#include "AssetLoader/AssetLoader.hpp"

namespace Vakol
{
    std::string AssetLoader::model_path = "assets/models/";
    std::string AssetLoader::texture_path = "assets/textures/";
    std::string AssetLoader::shader_path = "assets/shaders/";

    ModelLibrary AssetLoader::m_modelLibrary;
    MaterialLibrary AssetLoader::m_materialLibrary;

    Rendering::Assets::Model& AssetLoader::GetModel(const std::string& path, const float scale)
    {
        return m_modelLibrary.GetModel(path, scale);
    }

    Rendering::Assets::Material& AssetLoader::GetMaterial(const std::string& materialID)
    {
        return m_materialLibrary.GetMaterial(materialID);
    }

    void AssetLoader::AddTexture(const std::string& materialID, const Rendering::Assets::Texture& texture)
    {
        m_materialLibrary.AddTexture(materialID, texture);
    }

    void AssetLoader::AddMaterial(const Rendering::Assets::Material& material)
    {
        m_materialLibrary.AddMaterial(material);

        MaterialLibrary::SetupMaterial(material);
    }

    bool AssetLoader::GetTextures(const std::string& materialID, std::vector<Rendering::Assets::Texture>& textures)
    {
        return m_materialLibrary.GetTextures(materialID, textures);
    }

} // namespace Vakol

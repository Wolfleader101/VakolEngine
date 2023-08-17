#include "AssetLoader/AssetLoader.hpp"

namespace Vakol
{
    std::string AssetLoader::model_path = "assets/models/";
    std::string AssetLoader::texture_path = "assets/textures/";
    std::string AssetLoader::shader_path = "assets/shaders/";

    ModelLibrary AssetLoader::m_modelLibrary;

    Rendering::Assets::Model& AssetLoader::GetModel(const std::string& path, const float scale)
    {
        return m_modelLibrary.GetModel(path, scale);
    }
} // namespace Vakol

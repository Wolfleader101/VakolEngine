#include "AssetLoader.hpp"

#include <Controller/AssetLoader/TextureLoader.hpp>

namespace Vakol::Controller
{
    std::unordered_map<std::string, std::shared_ptr<Texture>> AssetLoader::m_TextureMap;
    std::unordered_map<std::string, std::shared_ptr<Model>> AssetLoader::m_ModelMap;
    std::unordered_map<std::string, std::shared_ptr<Shader>> AssetLoader::m_ShaderMap;


    Texture AssetLoader::GetTexture(const std::string& file) { return Texture(); }

}  // namespace Vakol::Controller

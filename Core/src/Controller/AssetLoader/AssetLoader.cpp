#include "AssetLoader.hpp"

#include <Controller/AssetLoader/ModelLoader.hpp>
#include <Controller/AssetLoader/ShaderLoader.hpp>
#include <Controller/AssetLoader/TextureLoader.hpp>

using namespace Vakol::Model::Assets;
namespace Vakol::Controller 
{

    std::string AssetLoader::model_path = "assets/models/";
    std::string AssetLoader::texture_path = "assets/textures/";
    std::string AssetLoader::shader_path = "assets/shaders/";

    std::unordered_map<std::string, std::shared_ptr<Assets::Texture>> AssetLoader::m_TextureMap;
    std::unordered_map<std::string, std::shared_ptr<Assets::Model>> AssetLoader::m_ModelMap;
    std::unordered_map<std::string, std::shared_ptr<Assets::Shader>> AssetLoader::m_ShaderMap;

    std::shared_ptr<Assets::Texture> AssetLoader::GetTexture(const std::string& file) 
    {
        std::shared_ptr<Assets::Texture> ret;

        auto iter = m_TextureMap.find(file);

        if (iter == m_TextureMap.end()) {
            ret = std::make_shared<Assets::Texture>(file);

            if (ret->GetID() == 0) return nullptr;  // if texture didn't load
            m_TextureMap[file] = ret;
        } else
            ret = m_TextureMap[file];

        return ret;
    }

    std::shared_ptr<Assets::Model> AssetLoader::GetModel(const std::string& file) 
    {
        std::shared_ptr<Assets::Model> ret;

        auto iter = m_ModelMap.find(file);

        if (iter == m_ModelMap.end()) {
            ret = std::make_shared<Assets::Model>(LoadModel(file));

            if (ret->GetMeshes().empty()) return nullptr;  // if model didn't load

            m_ModelMap[file] = ret;
        } else {
            ret = m_ModelMap[file];
        }
        return ret;
    }

    std::shared_ptr<Assets::Shader> AssetLoader::GetShader(const std::string& file) 
    {
        std::shared_ptr<Assets::Shader> ret;

        auto iter = m_ShaderMap.find(file);

        if (iter == m_ShaderMap.end()) {
            ret = std::make_shared<Assets::Shader>(file);

            if (ret->GetID() == 0) return nullptr;  // if shader didn't load

            m_ShaderMap[file] = ret;

        } else {
            ret = m_ShaderMap[file];
        }
        return ret;
    }

}  // namespace Vakol::Controller

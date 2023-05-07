#include "AssetLoader.hpp"

#include <Controller/AssetLoader/ModelLoader.hpp>
#include <Controller/AssetLoader/TextureLoader.hpp>

using namespace Assets;

namespace Vakol::Controller 
{
    std::string AssetLoader::model_path = "assets/models/";
    std::string AssetLoader::texture_path = "assets/textures/";
    std::string AssetLoader::shader_path = "assets/shaders/";

    std::unordered_map<std::string, std::shared_ptr<Assets::Texture>> AssetLoader::m_TextureMap;
    std::unordered_map<std::string, std::shared_ptr<Assets::Model>> AssetLoader::m_ModelMap;
    std::unordered_map<std::string, std::shared_ptr<Assets::Shader>> AssetLoader::m_ShaderMap;

    Assets::Texture& AssetLoader::GetTexture(std::string& file) 
    {
        std::shared_ptr<Assets::Texture> ret;

        if (const auto itr = m_TextureMap.find(file); itr == m_TextureMap.end()) 
        {
            ret = std::make_shared<Assets::Texture>(file);

            ret->SetID(LoadTexture(ret->path, false, false));

        	m_TextureMap[file] = ret;
        } else
            ret = m_TextureMap[file];

        return *ret;
    }

    std::shared_ptr<Assets::Model> AssetLoader::GetModel(const std::string& file) 
    {
        std::shared_ptr<Assets::Model> ret;

        const auto itr = m_ModelMap.find(file);

        if (itr == m_ModelMap.end()) {
            ret = std::make_shared<Assets::Model>(LoadModel(file));

            if (ret->meshes().empty())
            {
                VK_TRACE("no meshes found in model!");
        		return nullptr;  // if model didn't load
            }

            m_ModelMap[file] = ret;
        } else {
            ret = m_ModelMap[file];
        }
        return ret;
    }

    std::shared_ptr<Shader> AssetLoader::GetShader(const std::string& file) 
    {
        std::shared_ptr<Shader> ret;

        const auto itr = m_ShaderMap.find(file);

        if (itr == m_ShaderMap.end()) {
            ret = std::make_shared<Shader>(file);

            if (ret->GetID() == 0) return nullptr;  // if shader didn't load

            m_ShaderMap[file] = ret;

        } else {
            ret = m_ShaderMap[file];
        }
        return ret;
    }

}  // namespace Vakol::Controller

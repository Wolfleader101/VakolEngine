#include "AssetLoader.hpp"

#include <Controller/AssetLoader/ModelLoader.hpp>
#include <Controller/AssetLoader/TextureLoader.hpp>

using namespace Vakol::Model::Assets;

namespace Vakol::Controller 
{
    std::string AssetLoader::model_path = "assets/models/";
    std::string AssetLoader::texture_path = "assets/textures/";
    std::string AssetLoader::shader_path = "assets/shaders/";

    std::unordered_map<std::string, std::shared_ptr<Texture>> AssetLoader::m_TextureMap;
    std::unordered_map<std::string, std::shared_ptr<::Model>> AssetLoader::m_ModelMap;
    std::unordered_map<std::string, std::shared_ptr<Model::Shader>> AssetLoader::m_ShaderMap; 

    std::shared_ptr<Texture> AssetLoader::GetTexture(const std::string& file, const int size, const bool gamma, const bool flip, const void* data)
    {
	    auto ret = std::make_shared<Texture>();

        if (const auto itr = m_TextureMap.find(file); itr == m_TextureMap.end()) 
        {
            Texture texture{};

            texture.SetID(LoadTexture(size, gamma, flip, data));

            if(texture.GetID() == 0) return nullptr;  // if texture didn't load

            *ret = texture;

        	m_TextureMap[file] = ret;
        } else
            ret = m_TextureMap[file];

        return ret;
    }

    std::shared_ptr<Texture> AssetLoader::GetTexture(const std::string& file, const bool gamma, const bool flip) 
    {
	    auto ret = std::make_shared<Texture>();

        if (const auto itr = m_TextureMap.find(file); itr == m_TextureMap.end()) 
        {
            Texture texture{};
            texture.path = file;

            texture.SetID(LoadTexture(texture.path, gamma, flip));

            if(texture.GetID() == 0) return nullptr;  // if texture didn't load

            *ret = texture;

        	m_TextureMap[file] = ret;
        } else
            ret = m_TextureMap[file];

        return ret;
    }

    std::shared_ptr<Texture> AssetLoader::GetTexture(const std::string& file)
    {
	    auto ret = std::make_shared<Texture>();

        if (const auto itr = m_TextureMap.find(file); itr == m_TextureMap.end()) 
        {
            Texture texture{};
            texture.path = file;

            texture.SetID(LoadRawTexture(texture.path));

            if(texture.GetID() == 0) return nullptr;  // if texture didn't load

            *ret = texture;

        	m_TextureMap[file] = ret;
        } else
            ret = m_TextureMap[file];

        return ret;
    }

    std::shared_ptr<::Model> AssetLoader::GetModel(const std::string& file, const float scale, const bool backfaceCull) 
    {
        std::shared_ptr<::Model> ret;

        if (const auto itr = m_ModelMap.find(file); itr == m_ModelMap.end()) {
            ret = std::make_shared<::Model>(LoadModel(file, scale));

            if (ret->meshes().empty())
            {
                VK_TRACE("no meshes found in model!");
        		return nullptr;  // if model didn't load
            }

            m_ModelMap[file] = ret;
        } else {
            ret = m_ModelMap[file];
        }

        ret->SetCullBackface(backfaceCull); 

        return ret;
    }

    std::shared_ptr<Model::Shader> AssetLoader::GetShader(const std::string& file) 
    {
        std::shared_ptr<Model::Shader> ret;

        const auto itr = m_ShaderMap.find(file);

        if (itr == m_ShaderMap.end()) {
            ret = std::make_shared<Model::Shader>(file);

            if (ret->GetID() == 0) return nullptr;  // if shader didn't load

            m_ShaderMap[file] = ret;

        } else {
            ret = m_ShaderMap[file];
        }
        return ret;
    }

}  // namespace Vakol::Controller

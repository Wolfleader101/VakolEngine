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
    std::unordered_map<std::string, std::pair<std::shared_ptr<Model::Assets::Model>, std::shared_ptr<Animator>>> AssetLoader::m_ModelMap;
    std::unordered_map<std::string, std::shared_ptr<Animator>> AssetLoader::m_AnimatorMap;
    std::unordered_map<std::string, std::shared_ptr<Model::Shader>> AssetLoader::m_ShaderMap; 
    std::unordered_map<std::string, std::shared_ptr<Terrain>> AssetLoader::m_TerrainMap;

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

    std::shared_ptr<Animator> AssetLoader::GetAnimator(const std::string& file)
    {
        std::shared_ptr<Animator> ret = nullptr;

        if (const auto itr = m_AnimatorMap.find(file); itr == m_AnimatorMap.end())
        {
            VK_WARN("No Animator attached to the model {0}", file);
        }
        else
            ret = m_AnimatorMap[file];

        return ret;
    }

    Model::Assets::Animation AssetLoader::GetAnimation(const std::string& attached_model, const int state)
    {
        return GetAnimator(attached_model)->get(state);
    }

    std::pair<std::shared_ptr<Model::Assets::Model>, std::shared_ptr<Animator>> AssetLoader::GetModel(const std::string& file, const float scale, const bool animated, const bool backfaceCull)
    {
        bool instance;

	    return AssetLoader::GetModel(file, scale, animated, backfaceCull, instance);
    }

    std::pair<std::shared_ptr<::Model>, std::shared_ptr<Animator>> AssetLoader::GetModel(const std::string& file, const float scale, const bool animated, const bool backfaceCull, bool& instance) 
    {
        std::pair<std::shared_ptr<::Model>, std::shared_ptr<Animator>> ret;

        if (const auto itr = m_ModelMap.find(file); itr == m_ModelMap.end()) 
        {
            auto&& [model, animator] = LoadModel(file, scale, animated);

            ret.first = std::make_shared<::Model>(model);
            ret.second = std::make_shared<Animator>(animator);

            m_AnimatorMap[file] = ret.second;

            if (ret.first->meshes().empty())
                VK_ERROR("no meshes found in model!");

            m_ModelMap[file] = ret;

            instance = false;
        }
    	else
        {
            ret = m_ModelMap[file];

	        instance = true;
        }

        ret.first->SetCullBackface(backfaceCull); 

        return ret;
    }

    std::shared_ptr<Model::Shader> AssetLoader::GetShader(const std::string& file) 
    {
        std::shared_ptr<Model::Shader> ret;

        if (const auto itr = m_ShaderMap.find(file); itr == m_ShaderMap.end()) 
        {
            ret = std::make_shared<Model::Shader>(file);

            if (ret->GetID() == 0) return nullptr;  // if shader didn't load

            m_ShaderMap[file] = ret;

        } else {
            ret = m_ShaderMap[file];
        }
        return ret;
    }

    std::shared_ptr<Terrain> AssetLoader::GetTerrain(const std::string& name, const std::string& heightmap, float min, float max)
    {
        std::shared_ptr<Terrain> ret;

        if (const auto itr = m_TerrainMap.find(name); itr == m_TerrainMap.end()) 
        {
            ret = std::make_shared<Terrain>(LoadHeightMapTerrain(std::string(heightmap), min, max));

            if (!ret->GetSize()) return nullptr;  // if terrain didn't load

            m_TerrainMap[name] = ret;

        } else {
            ret = m_TerrainMap[name];
        }
        return ret;
    }

    std::shared_ptr<Terrain> AssetLoader::GetTerrain(const std::string& name)
    {
        if(const auto itr = m_TerrainMap.find(name); itr != m_TerrainMap.end())
            return itr->second;
        else
            return nullptr;
    }
}

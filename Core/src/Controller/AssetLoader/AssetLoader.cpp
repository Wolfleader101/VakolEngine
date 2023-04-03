#include "AssetLoader.hpp"

#include <Controller/AssetLoader/TextureLoader.hpp>
#include <Controller/AssetLoader/ModelLoader.hpp>
#include <Controller/AssetLoader/ShaderLoader.hpp>


namespace Vakol::Controller
{
    using Texture = Vakol::Model::Assets::Texture;
    using Model = Vakol::Model::Assets::Model;
    using Shader = Vakol::Model::Assets::Shader;

    std::string AssetLoader::model_path = "assets/models/";
    std::string AssetLoader::texture_path = "assets/textures/";
    std::string AssetLoader::shader_path = "assets/shaders/";

    std::unordered_map<std::string, std::shared_ptr<Texture>> AssetLoader::m_TextureMap;
    /*std::unordered_map<std::string, std::shared_ptr<Model>> AssetLoader::m_ModelMap;
    std::unordered_map<std::string, std::shared_ptr<Shader>> AssetLoader::m_ShaderMap;*/


    std::shared_ptr<Texture> AssetLoader::GetTexture(const std::string& file) 
    { 
        std::shared_ptr<Texture> ret; 

        auto iter = m_TextureMap.find(file);

        if (iter == m_TextureMap.end()) 
        {
            ret = std::make_shared<Texture>(LoadTexture(file));
        }
        else
            ret = m_TextureMap[file];

        return ret;
    }

    /*Model AssetLoader::GetModel(const std::string& file) { return Model(); }

    std::shared_ptr<Shader> AssetLoader::GetShader(const std::string& file) { return Shader(); }*/

}  // namespace Vakol::Controller

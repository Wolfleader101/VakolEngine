#include "AssetLoader.hpp"

#include <Controller/AssetLoader/ModelLoader.hpp>
#include <Controller/AssetLoader/ShaderLoader.hpp>
#include <Controller/AssetLoader/TextureLoader.hpp>
#include <Model/gl/GLShader.hpp>

namespace Vakol::Controller {
    using Texture = Vakol::Model::Assets::Texture;
    using Model = Vakol::Model::Assets::Model;
    using Shader = Vakol::Model::Assets::Shader;
    using GLShader = Vakol::Model::GLShader;

    std::string AssetLoader::model_path = "assets/models/";
    std::string AssetLoader::texture_path = "assets/textures/";
    std::string AssetLoader::shader_path = "assets/shaders/";

    std::unordered_map<std::string, std::shared_ptr<Texture>> AssetLoader::m_TextureMap;
    std::unordered_map<std::string, std::shared_ptr<Model>> AssetLoader::m_ModelMap;
    std::unordered_map<std::string, std::shared_ptr<Shader>> AssetLoader::m_ShaderMap;

    std::shared_ptr<Texture> AssetLoader::GetTexture(const std::string& file) {
        std::shared_ptr<Texture> ret;

        auto iter = m_TextureMap.find(file);

        if (iter == m_TextureMap.end()) {
            ret = std::make_shared<Texture>(LoadTexture(file));

            if (ret->id == 0) return nullptr;  // if texture didn't load
            m_TextureMap[file] = ret;
        } else
            ret = m_TextureMap[file];

        return ret;
    }

    std::shared_ptr<Model> AssetLoader::GetModel(const std::string& file) {
        std::shared_ptr<Model> ret;

        std::string path = model_path + file;

        auto iter = m_ModelMap.find(path);

        if (iter == m_ModelMap.end()) {
            ret = std::make_shared<Model>(LoadModel(path));

            if (ret->meshes.empty()) return nullptr;  // if model didn't load

            m_ModelMap[path] = ret;
        } else {
            ret = m_ModelMap[path];
        }
        return ret;
    }

    std::shared_ptr<Shader> AssetLoader::GetShader(const std::string& file) {
        std::shared_ptr<Shader> ret;

        auto iter = m_ShaderMap.find(file);

        if (iter == m_ShaderMap.end()) {
            ret = std::make_shared<GLShader>(file);

            if (ret->GetID() == 0) return nullptr;  // if shader didn't load

            m_ShaderMap[file] = ret;

        } else {
            ret = m_ShaderMap[file];
        }
        return ret;
    }

}  // namespace Vakol::Controller

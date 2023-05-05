#pragma once

#include <Model/Assets/Model.hpp>
#include <Model/Assets/Shader.hpp>
#include <Model/Assets/Texture.hpp>
#include <memory>
#include <unordered_map>

using namespace Vakol::Model;
namespace Vakol::Controller {

    class AssetLoader {
       public:
        // these can be changed with the game config
        static std::string model_path;
        static std::string texture_path;
        static std::string shader_path;

        static std::shared_ptr<Assets::Texture> GetTexture(std::string& file);
        static std::shared_ptr<Vakol::Model::Assets::Model> GetModel(const std::string& file);
        static std::shared_ptr<Assets::Shader> GetShader(const std::string& file);

       private:
        static std::unordered_map<std::string, std::shared_ptr<Assets::Texture>> m_TextureMap;
        static std::unordered_map<std::string, std::shared_ptr<Assets::Model>> m_ModelMap;
        static std::unordered_map<std::string, std::shared_ptr<Assets::Shader>> m_ShaderMap;
    };
}  // namespace Vakol::Controller

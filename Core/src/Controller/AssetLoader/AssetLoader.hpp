#pragma once

#include <unordered_map>

#include <Model/Assets/Model.hpp>
#include <Model/Assets/Texture.hpp>
#include <Model/Assets/Shader.hpp>

#include <memory>


namespace Vakol::Controller
{
    

    class AssetLoader {

        using Texture = Vakol::Model::Assets::Texture;
        using Model = Vakol::Model::Assets::Model;
        using Shader = Vakol::Model::Assets::Shader;

        public:

            static std::string model_path = "assets/models/";
            static std::string texture_path = "assets/textures/";
            static std::string shader_path = "assets/shaders/";


            static Texture GetTexture(const std::string& file);
            static Model GetModel(const std::string& file);
            static Shader GetShader(const std::string& file);


        private:

            static std::unordered_map<std::string, std::shared_ptr<Texture>> m_TextureMap;
            static std::unordered_map<std::string, std::shared_ptr<Model>> m_ModelMap;
            static std::unordered_map<std::string, std::shared_ptr<Shader>> m_ShaderMap;


        

    };
}
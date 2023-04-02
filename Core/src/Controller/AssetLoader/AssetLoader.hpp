#pragma once

#include <unordered_map>

#include <Model/Texture.hpp>
#include <Model/Model.hpp>

#include <memory>


namespace Vakol::Controller
{
    using Texture = Vakol::Model::Texture;
    using Model = Vakol::Model::Model;

    class AssetLoader {

        public:

            static Texture GetTexture(const std::string& file);
            static Model GetModel(const std::string& file);


        private:

        static Texture LoadTexture(const std::string& file);
        static Model LoadModel(const std::string& file);

        static std::unordered_map<std::string, std::shared_ptr<Texture>> m_TextureMap;
        static std::unordered_map <std::string, std::shared_ptr<Model>> m_ModelMap;


        

    };
}

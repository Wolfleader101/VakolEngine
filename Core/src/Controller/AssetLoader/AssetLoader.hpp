#pragma once

#include <Model/Assets/Model.hpp>
#include <Model/Shader.hpp>
#include <Model/Assets/Texture.hpp>

#include <Controller/Animator.hpp>

#include "Controller/Terrain.hpp"

#include <memory>
#include <unordered_map>

#include <Controller/Animator.hpp>

namespace Vakol::Controller
{
    class AssetLoader
	{
    public:
        // these can be changed with the game config
        static std::string model_path;
        static std::string texture_path;
        static std::string shader_path;

        static std::shared_ptr<Model::Assets::Texture> GetTexture(const std::string& file, int size, bool gamma, bool flip, const void* data); // embedded textures
        static std::shared_ptr<Model::Assets::Texture> GetTexture(const std::string& file, bool gamma, bool flip); // regular textures
		static std::shared_ptr<Model::Assets::Texture> GetTexture(const std::string& file); // raw textures

        static std::pair<std::shared_ptr<Model::Assets::Model>, std::shared_ptr<Animator>> GetModel(const std::string& file, float scale, bool animated, bool backfaceCull, bool& instance);
        static std::pair<std::shared_ptr<Model::Assets::Model>, std::shared_ptr<Animator>> GetModel(const std::string& file, float scale, bool animated, bool backfaceCull);

        static std::shared_ptr<Model::Shader> GetShader(const std::string& file);

        static std::shared_ptr<Terrain> GetTerrain(const std::string& name, const std::string& heightmap, float min, float max); //use when loading
        static std::shared_ptr<Terrain> GetTerrain(const std::string& name); //used once loaded

    private:
        static std::unordered_map<std::string, std::shared_ptr<Model::Assets::Texture>> m_TextureMap;
        static std::unordered_map<std::string, std::pair<std::shared_ptr<Model::Assets::Model>, std::shared_ptr<Animator>>> m_ModelMap;
        static std::unordered_map<std::string, std::shared_ptr<Model::Shader>> m_ShaderMap;
        static std::unordered_map<std::string, std::shared_ptr<Terrain>> m_TerrainMap;
    };
}  // namespace Vakol::Controller

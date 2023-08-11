#pragma once

#include <memory>
#include <unordered_map>

#include "Animation/Animator.hpp"
#include "Rendering/Assets/Model.hpp"
#include "Rendering/Assets/Texture.hpp"
#include "Rendering/Shader.hpp"

#include "Terrain/Terrain.hpp"

namespace Vakol
{
    /**
     * @brief AssetLoader class for loading game assets.
     */
    class AssetLoader
    {
      public:
        /**
         * @brief Path to the model files.
         */
        static std::string model_path;

        /**
         * @brief Path to the texture files.
         */
        static std::string texture_path;

        /**
         * @brief Path to the shader files.
         */
        static std::string shader_path;

        /**
         * @brief Get a texture with additional parameters for embedded textures.
         *
         * @param file The file path of the texture.
         * @param size The size of the texture.
         * @param gamma Flag indicating whether gamma correction should be applied.
         * @param flip Flag indicating whether the texture should be flipped vertically.
         * @param data Pointer to additional data for embedded textures.
         * @return std::shared_ptr<Assets::Texture> A shared pointer to the loaded texture.
         */
        static std::shared_ptr<Assets::Texture> GetTexture(const std::string& file, int size, bool gamma, bool flip,
                                                           const void* data); // embedded textures
        static std::shared_ptr<Assets::Texture> GetTexture(const std::string& file, bool gamma,
                                                           bool flip);               // regular textures
        static std::shared_ptr<Assets::Texture> GetTexture(const std::string& file); // raw textures

        static Assets::Animation GetAnimation(const std::string& file, const int state);
        static std::shared_ptr<Animator> GetAnimator(const std::string& file);

        static std::pair<std::shared_ptr<Assets::Model>, std::shared_ptr<Animator>> GetModel(const std::string& file,
                                                                                             float scale, bool animated,
                                                                                             bool backfaceCull,
                                                                                             bool& instance);
        static std::pair<std::shared_ptr<Assets::Model>, std::shared_ptr<Animator>> GetModel(const std::string& file,
                                                                                             float scale, bool animated,
                                                                                             bool backfaceCull);

        /**
         * @brief Get a shader.
         *
         * @param file The file path of the shader.
         * @return std::shared_ptr<Shader> A shared pointer to the loaded shader.
         */
        static std::shared_ptr<Shader> GetShader(const std::string& file);

        /**
         * @brief Get a terrain with specified parameters.
         *
         * @param name The name of the terrain.
         * @param heightmap The file path of the terrain heightmap.
         * @param min The minimum height of the terrain.
         * @param max The maximum height of the terrain.
         * @return std::shared_ptr<Terrain> A shared pointer to the loaded terrain.
         */
        static std::shared_ptr<Terrain> GetTerrain(const std::string& name, const std::string& heightmap, float min,
                                                   float max);

        /**
         * @brief Get a loaded terrain.
         *
         * @param name The name of the terrain.
         * @return std::shared_ptr<Terrain> A shared pointer to the loaded terrain.
         */
        static std::shared_ptr<Terrain> GetTerrain(const std::string& name);

      private:
        /**
         * @brief Map storing loaded textures.
         */
        static std::unordered_map<std::string, std::shared_ptr<Assets::Texture>> m_TextureMap;
        static std::unordered_map<std::string, std::pair<std::shared_ptr<Assets::Model>, std::shared_ptr<Animator>>>
            m_ModelMap;
        static std::unordered_map<std::string, std::shared_ptr<Animator>> m_AnimatorMap;
        static std::unordered_map<std::string, std::shared_ptr<Shader>> m_ShaderMap;

        /**
         * @brief Map storing loaded terrains.
         */
        static std::unordered_map<std::string, std::shared_ptr<Terrain>> m_TerrainMap;
    };

} // namespace Vakol

#pragma once

#include <Controller/Animator.hpp>
#include <Model/Assets/Model.hpp>
#include <Model/Assets/Texture.hpp>
#include <Model/Shader.hpp>
#include <memory>
#include <unordered_map>

#include "Controller/Terrain.hpp"

namespace Vakol::Controller {
    /**
     * @brief AssetLoader class for loading game assets.
     */
    class AssetLoader {
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
         * @return std::shared_ptr<Model::Assets::Texture> A shared pointer to the loaded texture.
         */
        static std::shared_ptr<Model::Assets::Texture> GetTexture(const std::string& file, int size, bool gamma,
                                                                  bool flip, const void* data);

        /**
         * @brief Get a regular texture.
         *
         * @param file The file path of the texture.
         * @param gamma Flag indicating whether gamma correction should be applied.
         * @param flip Flag indicating whether the texture should be flipped vertically.
         * @return std::shared_ptr<Model::Assets::Texture> A shared pointer to the loaded texture.
         */
        static std::shared_ptr<Model::Assets::Texture> GetTexture(const std::string& file, bool gamma, bool flip);

        /**
         * @brief Get a raw texture.
         *
         * @param file The file path of the texture.
         * @return std::shared_ptr<Model::Assets::Texture> A shared pointer to the loaded texture.
         */
        static std::shared_ptr<Model::Assets::Texture> GetTexture(const std::string& file);

        /**
         * @brief Get a model with additional parameters.
         *
         * @param file The file path of the model.
         * @param scale The scale factor for the model.
         * @param animated Flag indicating whether the model is animated.
         * @param backfaceCull Flag indicating whether backface culling should be applied.
         * @param instance Reference to a boolean variable indicating whether the model is an instance.
         * @return std::pair<std::shared_ptr<Model::Assets::Model>, std::shared_ptr<Animator>> A pair of shared pointers
         * to the loaded model and animator.
         */
        static std::pair<std::shared_ptr<Model::Assets::Model>, std::shared_ptr<Animator>> GetModel(
            const std::string& file, float scale, bool animated, bool backfaceCull, bool& instance);

        /**
         * @brief Get a model with specified parameters.
         *
         * @param file The file path of the model.
         * @param scale The scale factor for the model.
         * @param animated Flag indicating whether the model is animated.
         * @param backfaceCull Flag indicating whether backface culling should be applied.
         * @return std::pair<std::shared_ptr<Model::Assets::Model>, std::shared_ptr<Animator>> A pair of shared pointers
         * to the loaded model and animator.
         */
        static std::pair<std::shared_ptr<Model::Assets::Model>, std::shared_ptr<Animator>> GetModel(
            const std::string& file, float scale, bool animated, bool backfaceCull);

        /**
         * @brief Get a shader.
         *
         * @param file The file path of the shader.
         * @return std::shared_ptr<Model::Shader> A shared pointer to the loaded shader.
         */
        static std::shared_ptr<Model::Shader> GetShader(const std::string& file);

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
        static std::unordered_map<std::string, std::shared_ptr<Model::Assets::Texture>> m_TextureMap;

        /**
         * @brief Map storing loaded models and animators.
         */
        static std::unordered_map<std::string,
                                  std::pair<std::shared_ptr<Model::Assets::Model>, std::shared_ptr<Animator>>>
            m_ModelMap;

        /**
         * @brief Map storing loaded shaders.
         */
        static std::unordered_map<std::string, std::shared_ptr<Model::Shader>> m_ShaderMap;

        /**
         * @brief Map storing loaded terrains.
         */
        static std::unordered_map<std::string, std::shared_ptr<Terrain>> m_TerrainMap;
    };

}  // namespace Vakol::Controller

#pragma once


#include <memory>
#include <unordered_map>

#include "Rendering/Assets/Model.hpp"
#include "Rendering/Assets/Texture.hpp"

#include "ModelLibrary.hpp"
#include "MaterialLibrary.hpp"

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
         * \brief 
         * \param path 
         * \param scale 
         * \return 
         */
        static Rendering::Assets::Model& GetModel(const std::string& path, float scale = 1.0f);

        static Rendering::Assets::Material& GetMaterial(const std::string& materialID);

        /**
         * \brief dd
         * \param materialID 
         * \param texture 
         */
        static void AddTexture(const std::string& materialID, const Rendering::Assets::Texture& texture);

        /**
         * \brief 
         * \param material 
         */
        static void AddMaterial(const Rendering::Assets::Material& material);

        /**
         * \brief dd
         * \param materialID d
         * \param textures dd
         * \return bool true if valid, false if invalid
         */
        static bool GetTextures(const std::string& materialID, std::vector<Rendering::Assets::Texture>& textures);

      private:
        static ModelLibrary m_modelLibrary;
        static MaterialLibrary m_materialLibrary;
    };

} // namespace Vakol

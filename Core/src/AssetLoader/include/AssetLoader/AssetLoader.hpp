#pragma once

#include <memory>
#include <unordered_map>

#include "Rendering/Assets/Model.hpp"
#include "Rendering/Assets/Texture.hpp"

#include "MaterialLibrary.hpp"
#include "ModelLibrary.hpp"

// Needed a custom hasher for unordered-map key of std::pair.
struct PairHash
{
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const
    {
        const auto& v1 = std::hash<T1>{}(pair.first);
        const auto& v2 = std::hash<T2>{}(pair.second);

        return v1 ^ v2;
    }
};

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

        static Rendering::Assets::Texture& GetTexture(const std::string& path,
                                                      unsigned int type = Rendering::Assets::VK_TEXTURE_DIFFUSE);
        static Rendering::Assets::Texture& GetTexture(const std::string& path, unsigned int type, int size,
                                                      const void* buffer);

        /**
         * \brief dd
         * \param materialID
         * \param texture
         */
        static void AddTexture(const std::string& materialID, const Rendering::Assets::Texture& texture);

        /**
         * \brief
         * \param materialID
         * \param srcPath
         * \param dstPath
         * \param srcType
         * \param dstType
         */
        static void ReplaceTexture(const std::string& materialID, const std::string& srcPath,
                                   const std::string& dstPath, const std::string& type);

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

        static std::unordered_map<std::pair<std::string, unsigned int>, Rendering::Assets::Texture, PairHash> m_textures;
    };

} // namespace Vakol

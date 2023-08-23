#pragma once

#include <memory>
#include <unordered_map>

#include "Rendering/Assets/Model.hpp"
#include "Rendering/Assets/Texture.hpp"

#include "ModelLibrary.hpp"
#include "ShaderLibrary.hpp"
#include "TextureLibrary.hpp"

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
         * \return
         */
        static Rendering::Assets::Model& FindModel(const std::string& path);

        /**
         * \brief
         * \param path
         * \param scale
         * \return
         */
        static Rendering::Assets::Model& GetModel(const std::string& path, float scale = 1.0f);
        /**
         * \brief
         * \param modelID
         * \return
         */
        static const std::vector<Rendering::Assets::Mesh>& GetMeshes(const std::string& modelID);
        /**
         * \brief
         * \param shaderID id of the shader
         * \return
         */
        static unsigned int GetShader(const std::string& shaderID);
        /**
         * \brief
         * \param path
         * \param type the type of texture
         * \param levels mipmap levels
         * \return a texture
         */
        static Rendering::Assets::Texture& GetTexture(const std::string& path, unsigned int type, int levels = 1);
        /**
         * \brief
         * \param path
         * \param type
         * \param size the size of the buffer
         * \param data
         * \param levels mipmap levels
         * \return
         */
        static Rendering::Assets::Texture& GetTexture(const std::string& path, unsigned int type, int size,
                                                      const void* data, int levels = 1);
        /**
         * \brief
         * \param paths
         * \return
         */
        static std::vector<Rendering::Assets::Texture> GetTextures(std::vector<std::string>&& paths);

        /**
         * \brief
         * \param modelID
         * \param srcPath the path of the texture you want to replace
         * \param srcType the type of texture you want to replace
         * \param dstPath the new path of the texture
         * \param dstType the new type of texture
         */
        static void ReplaceTexture(const std::string& modelID, const std::string& srcPath, unsigned int srcType,
                                   const std::string& dstPath, unsigned int dstType);

      private:
        /**
         * \brief
         */
        static ModelLibrary m_modelLibrary;
        /**
         * \brief
         */
        static TextureLibrary m_textureLibrary;
    };

} // namespace Vakol

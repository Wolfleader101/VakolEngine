#pragma once

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
         * \brief finds a model based on a path
         * \param ID the path of the model
         * \return the model if found, else an error model
         */
        static Rendering::Assets::Model& FindModel(const GUID& ID);

        /**
         * \brief
         * \param ID the drawable ID
         * \param path the path of the model
         * \param scale the uniform scale of the model
         * \return
         */
        static Rendering::Assets::Model& GetModel(const GUID& ID, const std::string& path, float scale = 1.0f);
        /**
         * \brief
         * \param ID the drawable ID of the model
         * \return
         */
        static const std::vector<Rendering::Assets::Mesh>& GetMeshes(const GUID& ID);
        /**
         * \brief
         * \param path the path of the texture
         * \param type the type of texture
         * \param levels mipmap levels
         * \return a texture
         */
        static Rendering::Assets::Texture& GetTexture(const std::string& path, unsigned int type);
        /**
         * \brief
         * \param path the path of the texture
         * \param type the type of the texture
         * \param size the size of the buffer
         * \param data the buffer data address of the texture
         * \param levels mipmap levels
         * \return
         */
        static Rendering::Assets::Texture& GetTexture(const std::string& path, unsigned int type, int size,
                                                      const void* data);

        /**
         * \brief just grab the entire texture lol
         * \param path
         * \param type
         * \param width
         * \param height
         * \param channels
         * \param pixels
         */
        static Rendering::Assets::Texture& GetTexture(const std::string& path, unsigned int type, int& width,
                                                      int& height, int& channels, unsigned char*& pixels);

        /**
         * \brief
         * \param modelID
         * \param srcPath the path of the texture you want to replace
         * \param srcType the type of texture you want to replace
         * \param dstPath the new path of the texture
         * \param dstType the new type of texture
         */
        static void ReplaceTexture(const GUID& modelID, const std::string& srcPath, unsigned int srcType,
                                   const std::string& dstPath, unsigned int dstType);

        /**
         * \brief returns the status of a model, whether it exists or not.
         * \param modelID the model id
         * \return true or false
         */
        inline static bool IsExistingModel(const GUID& modelID)
        {
            return m_modelLibrary.ModelExists(modelID);
        }

      private:
        /**
         * \brief the model library containing a map of models to their paths
         */
        static ModelLibrary m_modelLibrary;
        /**
         * \brief the texture library containg a map of textures to their paths and types
         */
        static TextureLibrary m_textureLibrary;
    };

} // namespace Vakol
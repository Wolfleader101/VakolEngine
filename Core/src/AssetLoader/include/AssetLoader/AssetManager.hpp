#pragma once

// Standard C++ libraries
#include <algorithm>
#include <string>
#include <vector>

// Imported Libraries

// Vakol Libraries
#include "AssetLoader/FileLoader.hpp"
#include "AssetLoader/ModelProcessing.hpp"
#include "AssetLoader/ShaderProcessing.hpp"
#include "AssetLoader/TextureProcessing.hpp"

#include "Rendering/Assets/Model.hpp"
#include "Rendering/Assets/Shader.hpp"
#include "Rendering/Assets/Texture.hpp"

#include "Logger/Logger.hpp"

namespace Vakol
{
    /**
     * @class AssetManager
     * @brief A manager for all imported assets that are used in the engine
     */
    class AssetManager
    {
      public:
        /**
         * @enum FileType
         * @brief The FileType enum specifies the types of file resources used in the application.
         *
         * This enumeration is used throughout the AssetManager class to differentiate between
         * different kinds of file assets, such as models, textures, and shaders.
         */
        enum FileType
        {
            MODELS,   /**< Enum value MODELS. Used for model file assets. */
            TEXTURES, /**< Enum value TEXTURES. Used for texture file assets. */
            SHADERS   /**< Enum value SHADERS. Used for shader file assets. */
        };

        /**
         * @struct AssetIdentifier
         * @brief Holds information about an asset's usage and position in the cache.
         *
         * This structure is designed to keep track of the number of active references
         * to an asset, its position in the asset cache for quick lookup, and a list of
         * all GUIDs that reference this asset. Its to be used in conjunction with a map.
         */
        struct AssetIdentifier
        {
            /**
             * @brief Count of how many instances of this asset is currently being used
             */
            unsigned activeAssetCount = 0;
            /**
             * @brief The position of the asset in the cache vector
             */
            unsigned assetCachePosition = 0;
            /**
             * @brief A vector of GUIDs that reference the active instances of this asset
             */
            std::vector<GUID> assetIDs;
        };

        /**
         * @brief The AssetManager constructor
         */
        AssetManager();

        /**
         * @brief The AssetManager destructor
         */
        ~AssetManager();

        /**
         * @brief A function to return the path to an asset types folder
         * @param outputType The type of asset folder path to return
         * @return The path to the asset folder that was requested
         */
        static std::string GetFolderPath(AssetManager::FileType outputType);

        /**
         * @brief Finds a model based on it's GUID
         * @param ID The path of the model
         * @return The model if found, else an error model
         */
        static Rendering::Assets::Model& FindModel(const GUID& ID);

        /**
         * @brief Adds and returns a model
         * @param ID The drawable ID of the model
         * @param path The path of the model
         * @param scale The uniform scale of the model
         * @return The model that was just added
         */

        static Rendering::Assets::Model& GetModel(const GUID& ID, const std::string& path, float scale = 1.0f);
        /**
         * @brief Returns The meshes of a model
         * @param ID The drawable ID of the model
         * @return The meshes of the model
         */

        static const std::vector<Rendering::Assets::Mesh>& GetMeshes(const GUID& ID);
        /**
         * @brief Adds and returns a texture
         * @param Path The path of the texture
         * @param Type The type of texture
         * @return The texture that was just added
         */

        static Rendering::Assets::Texture& GetTexture(const std::string& path, unsigned int type);
        /**
         * @brief Adds and returns a texture, but with a buffer
         * @param path The path of the texture
         * @param type The type of the texture
         * @param size The size of the buffer
         * @param data The buffer data address of the texture
         * @return The texture that was just added
         */

        static Rendering::Assets::Texture& GetTexture(const std::string& path, unsigned int type, int size,
                                                      const void* data);

        /**
         * @brief Adds and returns a texture, but with the width, height, channels, and pixel data
         * @param path The path of the texture
         * @param type The type of the texture
         * @param width The width of the texture
         * @param height The height of the texture
         * @param channels The number of channels in the texture
         * @param pixels The pixel data of the texture
         */
        static Rendering::Assets::Texture& GetTexture(const std::string& path, unsigned int type, int& width,
                                                      int& height, int& channels, unsigned char*& pixels);

        /**
         * @brief Replaces the texture of a model
         * @param modelID The ID of the model
         * @param srcPath The path of the texture you want to replace
         * @param srcType The type of texture you want to replace
         * @param dstPath The new path of the texture
         * @param dstType The new type of texture
         */
        static void ReplaceTexture(const GUID& modelID, const std::string& srcPath, unsigned int srcType,
                                   const std::string& dstPath, unsigned int dstType);

        /**
         * @brief Returns the status of a model, whether it exists or not
         * @param modelID The model id
         * @return Returns a boolean value, true if the model exists, false if it doesn't
         */
        static bool IsExistingModel(const GUID& modelID);

      private:
        /**
         * @param modelPath
         * @brief The filepath to the model files
         */
        static std::string modelPath;

        /**
         * @param texturePath
         * @brief The filepath to the texture files
         */
        static std::string texturePath;

        /**
         * @param shaderPath
         * @brief Path to the shader files.
         */
        static std::string shaderPath;

        /**
         * @param m_modelProcessing
         * @brief The model processing class which contains the core functionality of loading, replacing and deleting
         * models
         */
        static ModelProcessing m_modelProcessing;

        /**
         * @param m_modelIdentifier
         * @brief A map which stores the GUID and vector position of a model in m_models
         *
         * The key is the file path of the model, and the value is the AssetIdentifier struct which
         * contains the data needed to keep track of the model in the m_models vector and its active instances.
         */
        static std::map<std::string, AssetIdentifier> m_modelIdentifier;

        /**
         * @param m_models
         * @brief A vector which contains the different models loaded into the engine
         */
        static std::vector<Rendering::Assets::Model> m_models;

        /**
         * @param m_textureProcessing
         * @brief The texture processing class which contains the core functionality of loading, replacing and deleting
         * textures
         */
        static TextureProcessing m_textureProcessing;

        /**
         * @param m_textureIdentifier
         * @brief A map which stores the GUID and vector position of a texture in m_textures
         *
         * The key is the file path of the texture, and the value is the AssetIdentifier struct which
         * contains the data needed to keep track of the texture in the m_textures vector and its active instances.
         */
        static std::map<std::string, AssetIdentifier> m_textureIdentifier;

        /**
         * @param m_textures
         * @brief A vector which contains the different textures loaded into the engine
         */
        static std::vector<Rendering::Assets::Texture> m_textures;

        /**
         * @param m_shaderProcessing
         * @brief The shader processing class which contains the core functionality of loading, replacing and deleting
         * shaders
         */
        static ShaderProcessing m_shaderProcessing;

        /**
         * @param m_shaderIdentifier
         * @brief A map which stores the GUID and vector position of a shader in m_shaders
         *
         * The key is the file path of the shader, and the value is the AssetIdentifier struct which
         * contains the data needed to keep track of the shader in the m_shaders vector and its active instances.
         */
        static std::map<std::string, AssetIdentifier> m_shaderIdentifier;

        /**
         * @param m_shaders
         * @brief A vector which contains the different shaders loaded into the engine
         */
        static std::vector<Rendering::Assets::Shader> m_shaders;
    } // namespace Vakol

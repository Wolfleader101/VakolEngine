#pragma once

// C++ Standard Libraries
#include <map>
#include <string>
#include <unordered_map>

// Imported Libraries
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

// Vakol Libraries
#include "Rendering/Assets/Animation.hpp"
#include "Rendering/Assets/Material.hpp"
#include "Rendering/Assets/Model.hpp"
#include "Rendering/Assets/Texture.hpp"
#include "Rendering/RenderCommand.hpp"
#include "Rendering/RenderData.hpp"

#include "AssetLoader/FileLoader.hpp"
#include "AssetLoader/ModelLoader.hpp"

#include "Logger/Logger.hpp"

#include "Math/Math.hpp"

namespace Vakol::Rendering::Assets
{
    struct Model;
}

namespace Vakol
{
    /**
     * @brief ModelProcessing is the actual storage unit for all models.
     */
    class ModelProcessing
    {
      public:
        /**
         * @brief Constructor for the ModelProcessing class
         */
        ModelProcessing();

        /**
         * @brief Find and return a reference to the model associated with the given GUID.
         *
         * @param ID The unique identifier (GUID) for the model to find.
         * @return Reference to the associated Rendering::Assets::Model object.
         */
        Rendering::Assets::Model& FindModel(const GUID& ID);

        /**
         * @brief Get the model associated with the given GUID and path. If the model does not exist, it will be loaded
         * from the specified path.
         *
         * @param ID The unique identifier (GUID) for the model to get or load.
         * @param path The file path to load the model from, if it does not already exist in the library.
         * @param scale The scale to apply to the model when loading it. Defaults to 1.0f.
         * @return Reference to the associated Rendering::Assets::Model object.
         */
        Rendering::Assets::Model& GetModel(const GUID& ID, const std::string& path, float scale = 1.0f);

        /**
         * @brief Check whether a model library is empty or not.
         * @return whether the model library contains no models in its map
         */
        bool IsEmpty() const;

        /**
         * @brief Check if a model exists in the library with the specified GUID.
         *
         * @param ID The unique identifier (GUID) to check for the existence of the associated model.
         * @return True if the model exists in the library, false otherwise.
         */
        bool ModelExists(const GUID& ID) const;

        /**
         * @brief Extracts the meshes from the given scene and stores them in the given vector
         *
         * @param scene The scene to extract the meshes from
         * @param meshes The vector to store the extracted meshes in
         */
        static void ExtractMeshes(const aiScene& scene, std::vector<Mesh>& meshes);

        /**
         * @brief Process the given mesh and return a Mesh object
         *
         * @param scene The scene to process the mesh from
         * @param mesh The mesh to process
         */
        static Mesh ProcessMesh(const aiScene& scene, const aiMesh& mesh);

        /**
         * @brief Extract the vertices from the given mesh and store them in the given vectors of vertices and indices
         *
         * @param mesh The mesh to extract the vertices from
         * @param vertices The vector to store the extracted vertices in
         * @param indices The vector to store the extracted indices in
         */
        static void ExtractVertices(const aiMesh& mesh, std::vector<Rendering::Vertex>& vertices,
                                    std::vector<unsigned int>& indices);

        /**
         * @brief Process the material from the given scene and store it in the given vector
         *
         * @param scene The scene to extract the material from
         * @param material The material to extract
         */
        static Material ProcessMaterial(const aiScene& scene, const aiMaterial* material);

        /**
         * @brief Extract the textures from the given scene and store them in the given vector
         *
         * @param scene The scene to extract the textures from
         * @param material The material to extract the textures from
         * @param type The type of texture to extract
         */
        static std::vector<Texture> ExtractTextures(const aiScene& scene, const aiMaterial* material,
                                                    aiTextureType type);

        /**
         * @brief Import the model from the given path and store it in the given model
         * @param model The model to store the imported model in
         * @param path The path of the model
         * @param scale The global scale of the model
         * @return Returns true if the model was successfully imported, false otherwise
         */
        bool ImportModel(Rendering::Assets::Model& model, const char* path, const float scale);

      private:
        /**
         * @brief Check if a unique model exists in the library with the specified path.
         *
         * @param path The file path to check for the existence of the associated unique model.
         * @return True if the unique model exists in the library, false otherwise.
         */
        bool UniqueModelExists(const std::string& path) const;

        /**
         * @brief Get the model associated with the given file path and scale.
         *
         * Note that this method is private and is used internally to manage models based on file path and scale.
         *
         * @param path The file path to get the associated model from.
         * @param scale The scale to consider while fetching the model.
         * @return Reference to the associated Rendering::Assets::Model object.
         */
        Rendering::Assets::Model& GetModel(const std::string& path, float scale);

        /**
         * @brief Get a reference to the error model with the specified scale.
         *
         * This is used internally to handle errors in model loading gracefully by providing a default error model.
         *
         * @param scale The scale to apply to the error model.
         * @return Reference to the error Rendering::Assets::Model object.
         */
        Rendering::Assets::Model& GetErrorModel(float scale);

        /**
         * @brief A map holding all the models loaded into the library, allowing for duplicate models based on the
         * drawable ID.
         */
        std::map<GUID, Rendering::Assets::Model> m_models; // duplicate models allowed based on the drawable ID

        /**
         * @brief An unordered map holding all the uniquely loaded models into the library, not allowing for duplicate
         * models based on the file path.
         */
        std::unordered_map<std::string, Rendering::Assets::Model> m_loadedModels; // duplicate models not allowed

        /**
         * @brief The default path for the error model
         */
        std::string ERROR_MODEL_PATH;
    };
} // namespace Vakol

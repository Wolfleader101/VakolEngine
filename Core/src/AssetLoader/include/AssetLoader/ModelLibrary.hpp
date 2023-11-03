#pragma once

#include <map>
#include <string>
#include <unordered_map>

#include "AssetLoader/ModelLoader.hpp"

#include "Logger/Logger.hpp"

#include "Rendering/Assets/Model.hpp"
#include "Rendering/RenderCommand.hpp"

namespace Vakol::Rendering::Assets
{
    struct Model;
}

namespace Vakol
{
    class GUID;

    /**
     * @brief ModelLibrary is the actual storage unit for all models.
     *
     */
    class ModelLibrary
    {
      public:
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
         * \brief Check whether a model library is empty or not.
         * \return whether the model library contains no models in its map
         */
        bool IsEmpty() const;

        /**
         * @brief Check if a model exists in the library with the specified GUID.
         *
         * @param ID The unique identifier (GUID) to check for the existence of the associated model.
         * @return True if the model exists in the library, false otherwise.
         */
        bool ModelExists(const GUID& ID) const;

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
    };
} // namespace Vakol
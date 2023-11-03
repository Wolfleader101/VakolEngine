#include "AssetLoader/ModelLibrary.hpp"

namespace Vakol
{
    const std::string ERROR_MODEL_PATH = "coreAssets/models/error.obj";

    Rendering::Assets::Model& ModelLibrary::FindModel(const GUID& ID)
    {
        if (!ModelExists(ID))
        {
            VK_ERROR("Unable to find unique model at ID: {0}", ID.ToString());
            return GetErrorModel(1.0f);
        }

        // VK_TRACE(m_models.at(ID).meshes.at(0).material.ID);

        return m_models.at(ID);
    }

    Rendering::Assets::Model& ModelLibrary::GetModel(const GUID& ID, const std::string& path, const float scale)
    {
        if (!ModelExists(ID))
        {
            Rendering::Assets::Model& model =
                GetModel(path, scale); // load the model and put it into the loaded models map

            for (Rendering::Assets::Mesh& mesh : model.meshes)
                mesh.material.ID = Rendering::GenerateID();

            m_models[ID] = model;
        }

        return m_models.at(ID);
    }

    Rendering::Assets::Model& ModelLibrary::GetModel(const std::string& path, const float scale)
    {
        if (!UniqueModelExists(path))
        {
            if (!ImportModel(m_loadedModels[path], path.c_str(), scale))
            {
                VK_ERROR("Unable to get model at path {0}", path);
                return GetErrorModel(scale);
            }
        }

        return m_loadedModels.at(path);
    }

    Rendering::Assets::Model& ModelLibrary::GetErrorModel(const float scale)
    {
        if (!UniqueModelExists(ERROR_MODEL_PATH))
        {
            if (!ImportModel(m_loadedModels[ERROR_MODEL_PATH], ERROR_MODEL_PATH.c_str(), scale))
            {
                VK_CRITICAL("ERROR MODEL NOT FOUND!");
            }
        }

        return m_loadedModels.at(ERROR_MODEL_PATH);
    }

    bool ModelLibrary::UniqueModelExists(const std::string& path) const
    {
        return m_loadedModels.find(path) != m_loadedModels.end();
    }

    bool ModelLibrary::ModelExists(const GUID& ID) const
    {
        return m_models.find(ID) != m_models.end();
    }

    bool ModelLibrary::IsEmpty() const
    {
        return m_models.empty();
    }

} // namespace Vakol
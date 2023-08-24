#include "AssetLoader/ModelLibrary.hpp"

#include "AssetLoader/ModelLoader.hpp"

#include "Rendering/Assets/Model.hpp"

#include "Logger/Logger.hpp"

namespace Vakol
{
    const std::string ERROR_MODEL_PATH = "coreAssets/models/error.obj";

    Rendering::Assets::Model& ModelLibrary::FindModel(const std::string& path)
    {
        if (m_models.find(path) == m_models.end())
        {
            VK_ERROR("Unable to find model at path {0}", path);

            return GetErrorModel(1.0f);
        }

        return m_models.at(path);
    }

    Rendering::Assets::Model& ModelLibrary::GetModel(const std::string& path, const float scale)
    {
        if (m_models.find(path) == m_models.end())
        {
            bool success = true;

            const auto& model = ImportModel(path.c_str(), scale, success);

            if (success)
            {
                m_models[path] = model;

                return m_models.at(path);
            }

            VK_ERROR("Unable to get model at path {0}", path);

            return GetErrorModel(scale);
        }

        return m_models.at(path);
    }

    Rendering::Assets::Model& ModelLibrary::GetErrorModel(const float scale)
    {
        bool success;

        const auto& error = ImportModel(ERROR_MODEL_PATH.c_str(), scale, success);

        if (success && m_models.find(ERROR_MODEL_PATH) == m_models.end())
        {
            m_models[ERROR_MODEL_PATH] = error;
        }

        return m_models.at(ERROR_MODEL_PATH);
    }

} // namespace Vakol
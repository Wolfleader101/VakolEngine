#include "AssetLoader/ModelLibrary.hpp"

#include "AssetLoader/ModelLoader.hpp"

#include "Rendering/Assets/Model.hpp"

#include "Logger/Logger.hpp"

namespace Vakol
{
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

            const auto& error = ImportModel("coreAssets/models/error.obj", scale, success);

            if (success)
            {
                m_models[path] = error;

                return m_models.at(path);
            }
        }

        return m_models.at(path);
    }

} // namespace Vakol
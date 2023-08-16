#include "Rendering/ModelLibrary.hpp"

#include "Rendering/Assets/ModelImporter.hpp"

#include "Rendering/Assets/Model.hpp"

#include "Logger/Logger.hpp"

namespace Vakol::Rendering
{
    std::unordered_map<std::string, Assets::Model> ModelLibrary::m_models;

    Assets::Model ModelLibrary::GetModel(const std::string& path, const float scale)
    {
        if (m_models.find(path) == m_models.end())
        {
            bool success = true;

            auto&& model = Assets::ImportModel(path.c_str(), scale, success);

            if (success)
            {
                m_models[path] = model;

                return model;
            }

            VK_ERROR("Unable to get model at path {0}", path);
        }

        return m_models.at(path);
    }

}
#include "ModelLibrary.hpp"

#include "Importer/ModelImporter.hpp"

#include "Model.hpp"

#include "Controller/Logger.hpp"

namespace Vakol::Rendering::Assets
{
    std::unordered_map<std::string, Model> ModelLibrary::m_models;

    Model ModelLibrary::GetModel(const std::string& path, const float scale)
    {
        if (m_models.find(path) == m_models.end())
        {
            bool success = true;

            Model model = Importer::ImportModel(path.c_str(), scale, success);

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
#pragma once

#include <string>
#include <map>
#include <unordered_map>

namespace Vakol::Rendering::Assets
{
    struct Model;
}

namespace Vakol
{
    class GUID;

    class ModelLibrary
    {
      public:
        Rendering::Assets::Model& FindModel(const GUID& ID);
        Rendering::Assets::Model& GetModel(const GUID& ID, const std::string& path, float scale = 1.0f);

      private:
        [[nodiscard]] bool ModelExists(const GUID& ID) const;
        [[nodiscard]] bool UniqueModelExists(const std::string& path) const;

        Rendering::Assets::Model& GetModel(const std::string& path, float scale);

        Rendering::Assets::Model& GetErrorModel(float scale);

        std::map<GUID, Rendering::Assets::Model>
            m_models; // duplicate models allowed based on the drawable ID
        std::unordered_map<std::string, Rendering::Assets::Model> m_loadedModels; // duplicate models not allowed
    };
} // namespace Vakol
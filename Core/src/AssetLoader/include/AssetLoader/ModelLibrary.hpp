#pragma once

#include <string>
#include <unordered_map>

namespace Vakol::Rendering::Assets
{
    struct Model;
}

namespace Vakol
{
    class ModelLibrary
    {
      public:
        Rendering::Assets::Model& FindModel(const std::string& ID);
        Rendering::Assets::Model& GetModel(const std::string& ID, const std::string& path, float scale = 1.0f);

      private:
        [[nodiscard]] bool ModelExists(const std::string& ID) const;
        [[nodiscard]] bool UniqueModelExists(const std::string& path) const;

        Rendering::Assets::Model& GetModel(const std::string& path, float scale);

        Rendering::Assets::Model& GetErrorModel(float scale);

        std::unordered_map<std::string, Rendering::Assets::Model> m_models; // duplicate models allowed based on the drawable ID
        std::unordered_map<std::string, Rendering::Assets::Model> m_loadedModels; // duplicate models not allowed
    };
} // namespace Vakol
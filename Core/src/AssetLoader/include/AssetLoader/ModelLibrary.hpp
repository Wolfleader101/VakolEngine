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
        Rendering::Assets::Model& GetModel(const std::string& path, float scale);

      private:
        std::unordered_map<std::string, Rendering::Assets::Model> m_models;
    };
} // namespace Vakol::Rendering
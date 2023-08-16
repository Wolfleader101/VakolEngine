#pragma once

#include <string>
#include <unordered_map>

namespace Vakol::Rendering::Assets
{
    struct Model;
}

namespace Vakol::Rendering
{

    class ModelLibrary
    {
      public:
        static Assets::Model GetModel(const std::string& path, float scale = 1.0f);

      private:
        static std::unordered_map<std::string, Assets::Model> m_models;
    };
} // namespace Vakol::Rendering
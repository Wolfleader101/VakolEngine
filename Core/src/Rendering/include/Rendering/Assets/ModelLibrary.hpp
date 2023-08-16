#pragma once

#include <unordered_map>
#include <string>

namespace Vakol::Rendering::Assets
{
    struct Model;

    class ModelLibrary
    {
    public:
        static Model GetModel(const std::string& path, float scale = 1.0f);
    private:
        static std::unordered_map<std::string, Model> m_models;
    };
}
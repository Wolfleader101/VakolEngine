#pragma once

#include <unordered_map>

#include "Math/Math.hpp"

#include <string>

namespace Vakol::Rendering::Assets
{
    struct Material;
}

namespace Vakol::Rendering
{
    class MaterialLibrary
    {
    public:
        static void AddMaterial(const Assets::Material& material);

        static void SetColor(unsigned int shader, const Math::Vec4& color);
        static void SetShininess(unsigned int shader, float shininess);
    private:
        static std::unordered_map<std::string, Assets::Material> m_materials;
    };
}
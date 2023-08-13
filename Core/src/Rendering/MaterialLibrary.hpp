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

        static void AddTexture(const Assets::Material& material, const std::string& texturePath, unsigned int texture);
        static unsigned int GetTexture(const Assets::Material& material, const std::string& texturePath);

        static void SetColor(const Assets::Material& material, const Math::Vec4& color);
        static void SetShininess(const Assets::Material& material, float shininess);
    private:
        static std::unordered_map<std::string, Assets::Material> m_materials;
        static std::unordered_map<std::pair<std::string, std::string>, unsigned int> m_textures;
    };
}
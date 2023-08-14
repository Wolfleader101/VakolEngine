#pragma once

#include <unordered_map>

#include "Math/Math.hpp"

#include <string>

namespace Vakol::Rendering::Assets
{
    struct Material;
    struct Texture;
}

namespace Vakol::Rendering
{
    class MaterialLibrary
    {
    public:
        static void SetupMaterial(const Assets::Material& material);

        static void AddMaterial(const Assets::Material& material);
        static Assets::Material& GetMaterial(const std::string& materialID);

        static void AddTexture(const std::string& materialID, const Assets::Texture& texture);

        static std::vector<Assets::Texture>& GetTextures(const std::string& materialID);
        static int GetTextureCount(const std::string& materialID);

        static void SetColor(const std::string& materialID, const Math::Vec4& color);
        static void SetShininess(const std::string& materialID, float shininess);
    private:
        static std::unordered_map<std::string, Assets::Material> m_materials;
        static std::unordered_map<std::string, std::vector<Assets::Texture>> m_textures;
    };
}
#pragma once

#include <unordered_map>

#include "Math/Math.hpp"

#include <string>

namespace Vakol::Rendering::Assets
{
    struct Material;
    struct Texture;
} // namespace Vakol::Rendering::Assets

namespace Vakol
{
    class MaterialLibrary
    {
      public:
        static void SetupMaterial(const Rendering::Assets::Material& material);

        static void AddMaterial(const Rendering::Assets::Material& material);
        static Rendering::Assets::Material& GetMaterial(const std::string& materialID);

        static void AddTexture(const std::string& materialID, const Rendering::Assets::Texture& texture);

        static bool GetTextures(const std::string& materialID, std::vector<Rendering::Assets::Texture>& textures);
        static int GetTextureCount(const std::string& materialID);

        static void SetColor(const std::string& materialID, const Math::Vec4& color);
        static void SetShininess(const std::string& materialID, float shininess);

      private:
        static std::unordered_map<std::string, Rendering::Assets::Material> m_materials;
        static std::unordered_map<std::string, std::vector<Rendering::Assets::Texture>> m_textures;
    };
} // namespace Vakol::Rendering
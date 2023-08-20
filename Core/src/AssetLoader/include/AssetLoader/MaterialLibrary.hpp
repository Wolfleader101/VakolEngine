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

        void AddMaterial(const Rendering::Assets::Material& material);
        Rendering::Assets::Material& GetMaterial(const std::string& materialID);

        void AddTexture(const std::string& materialID, const Rendering::Assets::Texture& texture);
        void ReplaceTexture(const std::string& materialID, const std::string& srcPath, const std::string& dstPath,
                            unsigned int type);

        bool GetTextures(const std::string& materialID, std::vector<Rendering::Assets::Texture>& textures);
        int GetTextureCount(const std::string& materialID);

        void SetColor(const std::string& materialID, const Math::Vec4& color);
        void SetShininess(const std::string& materialID, float shininess);

      private:
        std::unordered_map<std::string, Rendering::Assets::Material> m_materials;
        std::unordered_map<std::string, std::vector<Rendering::Assets::Texture>> m_textures;
    };
} // namespace Vakol
#include "AssetLoader/MaterialLibrary.hpp"

#include "AssetLoader/ShaderLibrary.hpp"

#include "Logger/Logger.hpp"
#include "Rendering/Assets/Material.hpp"

namespace Vakol
{
    std::unordered_map<std::string, Rendering::Assets::Material> MaterialLibrary::m_materials;
    std::unordered_map<std::string, std::vector<Rendering::Assets::Texture>> MaterialLibrary::m_textures;

    void MaterialLibrary::SetupMaterial(const Rendering::Assets::Material& material)
    {
        ShaderLibrary::SetVec3(ShaderLibrary::GetShader(material.shaderID), "material.ambient_color",
                               material.properties.ambient_color);
        ShaderLibrary::SetVec3(ShaderLibrary::GetShader(material.shaderID), "material.diffuse_color",
                               material.properties.diffuse_color);
        ShaderLibrary::SetVec3(ShaderLibrary::GetShader(material.shaderID), "material.specular_color",
                               material.properties.specular_color);
    }

    void MaterialLibrary::AddMaterial(const Rendering::Assets::Material& material)
    {
        if (m_materials.find(material.ID) == m_materials.end())
        {
            m_materials[material.ID] = material;
            m_textures.emplace();
        }
    }

    Rendering::Assets::Material& MaterialLibrary::GetMaterial(const std::string& materialID)
    {
        if (m_materials.find(materialID) != m_materials.end())
            return m_materials.at(materialID);
    }

    void MaterialLibrary::AddTexture(const std::string& materialID, const Rendering::Assets::Texture& texture)
    {
        m_textures[materialID].emplace_back(texture);
    }

    bool MaterialLibrary::GetTextures(const std::string& materialID, std::vector<Rendering::Assets::Texture>& textures)
    {
        if (m_textures.find(materialID) != m_textures.end())
        {
            textures = m_textures.at(materialID);

            return true;
        }

        return false;
    }

    int MaterialLibrary::GetTextureCount(const std::string& materialID)
    {
        if (m_textures.find(materialID) != m_textures.end())
            return m_textures.at(materialID).size();
    }

    void MaterialLibrary::SetColor(const std::string& materialID, const Math::Vec4& color)
    {
        ShaderLibrary::SetVec4(ShaderLibrary::GetShader(GetMaterial(materialID).shaderID), "material.color", color);
    }

    void MaterialLibrary::SetShininess(const std::string& materialID, const float shininess)
    {
        ShaderLibrary::SetFloat(ShaderLibrary::GetShader(GetMaterial(materialID).shaderID), "material.shininess",
                                shininess);
    }
} // namespace Vakol::Rendering

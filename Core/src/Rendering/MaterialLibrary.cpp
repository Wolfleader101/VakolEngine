#include "MaterialLibrary.hpp"

#include "ShaderLibrary.hpp"
#include "Assets/Material.hpp"
#include "Controller/Logger.hpp"

namespace Vakol::Rendering
{
    std::unordered_map<std::string, Assets::Material> MaterialLibrary::m_materials;
    std::unordered_map<std::pair<std::string, std::string>, unsigned int> MaterialLibrary::m_textures;

    void MaterialLibrary::AddMaterial(const Assets::Material& material)
    {
        if (m_materials.find(material.ID) == m_materials.end())
            m_materials[material.ID] = material;
    }

    void MaterialLibrary::AddTexture(const Assets::Material& material, const std::string& texturePath, const unsigned int texture)
    {
        if (m_materials.find(material.ID) != m_materials.end())
        {
            if (m_textures.find(std::make_pair(material.ID, texturePath)) == m_textures.end())
                m_textures[std::make_pair(material.ID, texturePath)] = texture;
        }
    }

    unsigned int MaterialLibrary::GetTexture(const Assets::Material& material, const std::string& texturePath)
    {
        if (m_materials.find(material.ID) != m_materials.end())
        {
            if (m_textures.find(std::make_pair(material.ID, texturePath)) != m_textures.end())
                return m_textures.at(std::make_pair(material.ID, texturePath));

            VK_ERROR("Unable to find texture in material!");
        }
        else
            VK_ERROR("Unable to find material at ID = {0}", material.ID);

        return 0u; // fail
    }

    void MaterialLibrary::SetColor(const Assets::Material& material, const Math::Vec4& color)
    {
        ShaderLibrary::SetVec4(ShaderLibrary::GetShader(material.shaderID), "material.color", color);
    }

    void MaterialLibrary::SetShininess(const Assets::Material& material, const float shininess)
    {
        ShaderLibrary::SetFloat(ShaderLibrary::GetShader(material.shaderID), "material.shininess", shininess);
    }
}

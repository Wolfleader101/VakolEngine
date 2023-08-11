#include "MaterialLibrary.hpp"

#include "ShaderLibrary.hpp"
#include "Assets/Material.hpp"

namespace Vakol::Rendering
{
    std::unordered_map<std::string, Assets::Material> MaterialLibrary::m_materials;

    void MaterialLibrary::AddMaterial(const Assets::Material& material)
    {
        if (m_materials.find(material.name) == m_materials.end())
            m_materials[material.name] = material;
    }

    void MaterialLibrary::SetColor(const unsigned int shader, const Math::Vec4& color)
    {
        ShaderLibrary::SetVec4(shader, "material.color", color);
    }

    void MaterialLibrary::SetShininess(const unsigned int shader, const float shininess)
    {
        ShaderLibrary::SetFloat(shader, "material.shininess", shininess);
    }
}

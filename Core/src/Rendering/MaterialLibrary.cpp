#include "MaterialLibrary.hpp"

#include "ShaderLibrary.hpp"

namespace Vakol::Rendering
{
    void MaterialLibrary::AddMaterial(const Assets::Material& material)
    {
        
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
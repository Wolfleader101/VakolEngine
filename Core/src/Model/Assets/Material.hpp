#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Shader.hpp"
#include "Texture.hpp"

namespace Vakol::Model::Assets {

    struct MaterialSpec 
    {
        MaterialSpec() = default;

        glm::vec3 AMBIENT = glm::vec3(1.0f);
        glm::vec3 DIFFUSE = glm::vec3(1.0f);
        glm::vec3 SPECULAR = glm::vec3(0.5f);
        glm::vec3 EMISSIVE = glm::vec3(0.0f);
        float SHININESS = 32.0f;

        std::vector<Texture> textures;
    };

    class Material 
    {
    public:
        Material(const MaterialSpec& spec) : m_spec(spec){};

        virtual const int GetTextureCount() const = 0;
        virtual std::vector<Texture> textures() = 0;

    protected:
        MaterialSpec m_spec;
    };
}  // namespace Vakol::Model::Assets
#pragma once


#include <glm/common.hpp>
#include <glm/glm.hpp>

#include "Texture.hpp"



namespace Vakol::Model::Assets 
{

    struct MaterialInfo {
        glm::vec3 _AMBIENT;
        glm::vec3 _DIFFUSE;
        glm::vec3 _SPECULAR;
        glm::vec3 _EMISSIVE;

        float _SHININESS;

        std::vector<Texture> textures;
    };

    struct Material {
        Material() = default;
        Material(const std::vector<Texture>& textures) : textures(textures){};

        glm::vec3 AMBIENT = glm::vec3(0.0f);
        glm::vec3 DIFFUSE = glm::vec3(0.0f);
        glm::vec3 SPECULAR = glm::vec3(0.0f);
        glm::vec3 EMISSIVE = glm::vec3(0.0f);
        float SHININESS = 0.0f;

        std::vector<Texture> textures;
    };
}  // namespace Vakol::Model::Assets
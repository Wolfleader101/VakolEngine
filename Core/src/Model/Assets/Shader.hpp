#pragma once

#include <glm/mat4x4.hpp>
#include <string>

namespace Vakol::Model::Assets {
    class Shader {
       public:
        Shader(const unsigned int id) : id(id){};
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        const unsigned int GetID() const { return id; }

       protected:
        unsigned int id;
    };
}  // namespace Vakol::Model
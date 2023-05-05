#pragma once

#include <string>

namespace Vakol::Model::Assets 
{   
    class Texture
    {
    public:
        Texture() = default;

        explicit Texture(std::string&& path) : path(std::move(path)) {}
        explicit Texture(std::string& path) : path(std::move(path)) {}

        void SetID(const unsigned int id) { this->m_ID = id; }
        [[nodiscard]] unsigned int GetID() const { return this->m_ID; }

        std::string path;
        std::string type;
    private:
        unsigned int m_ID = 0;
    };
}
#pragma once

#include <string>

namespace Vakol::Model::Assets 
{   
    class Texture
    {
    public:
        Texture() = default;

        Texture(std::string&& path) : m_path(std::move(path)) {}
        Texture(std::string& path) : m_path(std::move(path)) {}

        ~Texture() {}

        [[nodiscard]] unsigned int GetId() const { return this->m_ID; }

        [[nodiscard]] const std::string& GetPath() const { return this->m_path; }
        [[nodiscard]] const std::string& GetType() const { return this->m_type; }

        void SetPath(const std::string& path) { this->m_path = path; }
        void SetType(const std::string& type) { this->m_type = type; }

    private:
        unsigned int m_ID = 0;

        std::string m_path = "";
        std::string m_type = "";
    };
}
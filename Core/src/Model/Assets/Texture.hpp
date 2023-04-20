#pragma once

#include <Controller/AssetLoader/TextureLoader.hpp>
#include <string>

namespace Vakol::Model::Assets 
{   
    using Controller::LoadTexture;
    using Controller::LoadRawTexture;

    class Texture
    {
    public:
        Texture() = default;
        Texture(const std::string& path) : m_path(path), m_ID(LoadRawTexture(path)) {};
        Texture(const std::string& path, const bool gamma, const bool flip) : m_path(path), m_ID(LoadTexture(path, gamma, flip)) {};

        ~Texture() = default;

        void Bind(const unsigned int slot) const;
        void Unbind(const unsigned int slot) const;

        const unsigned int GetID() const { return this->m_ID; }

        const std::string GetPath() const { return this->m_path; }
        const std::string GetType() const { return this->m_type; }

        void SetPath(const std::string& path) { this->m_path = path; }
        void SetType(const std::string& type) { this->m_type = type; }
        //virtual void SetData(const int width, const int height, const unsigned int format, const void* data) const = 0;

    private:
        unsigned int m_ID;

        std::string m_path;
        std::string m_type;
    };
}
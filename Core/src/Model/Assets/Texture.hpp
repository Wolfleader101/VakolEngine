#pragma once

#include <Controller/AssetLoader/TextureLoader.hpp>
#include <string>

namespace Vakol::Model::Assets 
{   
    using Controller::LoadTexture;
    using Controller::LoadRawTexture;
    using Controller::LoadNoiseTexture;

    class Texture
    {
    public:
        Texture() = default;

        explicit Texture(const std::string& path) : m_ID(LoadRawTexture(path)), m_path(path) {}
        
        Texture(const std::string& path, const bool gamma, const bool flip) : m_ID(LoadTexture(path, gamma, flip)),
                                                                              m_path(path) {}
        
        Texture(const int size, float scale, const int octaves, const float persistence, const float lacunarity) : 
            m_ID(LoadNoiseTexture(size, scale, octaves, persistence, lacunarity)) {}

        ~Texture() = default;

        void Bind(const unsigned int slot) const;
        void Unbind(const unsigned int slot) const;

        [[nodiscard]] unsigned int GetId() const { return this->m_ID; }

        [[nodiscard]] const std::string& GetPath() const { return this->m_path; }
        [[nodiscard]] const std::string& GetType() const { return this->m_type; }

        void SetPath(const std::string& path) { this->m_path = path; }
        void SetType(const std::string& type) { this->m_type = type; }
        //virtual void SetData(const int width, const int height, const unsigned int format, const void* data) const = 0;

    private:
        unsigned int m_ID;

        std::string m_path;
        std::string m_type;
    };
}
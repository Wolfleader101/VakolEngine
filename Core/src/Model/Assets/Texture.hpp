#pragma once

#include <Controller/AssetLoader/TextureLoader.hpp>
#include <string>

namespace Vakol::Model::Assets {
    class Texture {
       public:
        Texture() = default;
        
        Texture(const int width, const int height, const unsigned int format)
            : m_width(width), m_height(height), m_format(format) {}
        
        Texture(const std::string& path, const bool raw = false) {
            if (raw)
                this->m_ID = Controller::LoadRawTexture(path);
            else
                this->m_ID = Controller::LoadTexture(path);
        }

        const unsigned int id() const { return m_ID; }

        std::string type;
        std::string path;

    protected:
        unsigned int m_ID = 0;
        unsigned int m_format = 0;

        int m_width = 0;
        int m_height = 0;
    };

}  // namespace Vakol::Model
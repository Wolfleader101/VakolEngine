#pragma once

#include <Model/Assets/Texture.hpp>

namespace Vakol::Model::Assets {
    class GLTexture : public Texture {
       public:
        GLTexture() = default;
        GLTexture(const std::string& path, const bool raw = false, const bool gamma = false, const bool flip = true) : Texture(path, raw, gamma, flip){};
        GLTexture(const int width, const int height, const unsigned int format);

        void Bind(const unsigned int unit = 0) const;

        void SetData(const void* data);
    };
}  // namespace Vakol::Model::Asset
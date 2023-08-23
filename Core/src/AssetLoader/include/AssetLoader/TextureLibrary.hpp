#pragma once

#include <string>
#include <vector>

#include <unordered_map>

struct PairHash
{
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const
    {
        const auto& v1 = std::hash<T1>{}(pair.first);
        const auto& v2 = std::hash<T2>{}(pair.second);

        return v1 ^ v2;
    }
};

namespace Vakol
{
    namespace Rendering::Assets
    {
        struct Texture;
    }

    class TextureLibrary
    {
      public:
        Rendering::Assets::Texture& GetTexture(const std::string& path, unsigned int type, int levels);
        Rendering::Assets::Texture& GetTexture(const std::string& path, unsigned int type, int size, const void* data,
                                               int levels);

      private:
        Rendering::Assets::Texture& GetErrorTexture(unsigned int type);

        std::unordered_map<std::pair<std::string, unsigned int>, Rendering::Assets::Texture, PairHash> m_textures;

        [[nodiscard]] bool FindTexture(const std::string& path, unsigned int type) const;
    };
} // namespace Vakol
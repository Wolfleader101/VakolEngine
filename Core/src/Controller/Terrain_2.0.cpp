#include "Terrain_2.0.hpp"

#include <Controller/AssetLoader/FileLoader.hpp>
#include <Controller/Logger.hpp>

namespace Vakol::Controller
{
    Model::Assets::Mesh Terrain_2::LoadHeightMap(const std::string& path) const
    {
        std::vector<Vertex> vertices;

        int width = 0, height = 0;
        auto data = LoadImage(path, width, height);

        const auto size = width | height;
        vertices.reserve(size * size);

        for (int z = 0; z < size; ++z)
        {
            for (int x = 0; x < size; ++x)
            {
                const auto pixel_offset = data + (z * size + x);
                const auto y = pixel_offset[0];

                vertices.push_back({
                    glm::vec3((-size / 2.0f + size * x / static_cast<float>(size)) * 1.0f,
                              (y * 0.1f - 16.0f),
                              (-size / 2.0f + size * z / static_cast<float>(size)) * 1.0f),
                    glm::vec3(0.0f), // normal
                    glm::vec2((x / static_cast<float>(size)),
                              (z / static_cast<float>(size))),
                    glm::vec3(0.0f), // tangent
                    glm::vec3(0.0f)  // bitangent
                });
            }
        }
        
        delete [] data;
        data = nullptr; // don't need the data anymore

        const int LEVEL_OF_DETAIL = 1; // HIGHER VALUE = LOWER RESOLUTION

        std::vector<unsigned int> indices;

        indices.reserve((size - 1) * size * 2);

        for (int i = 0; i < size - 1; i += LEVEL_OF_DETAIL)
        {
            for (int j = 0; j < size; j += LEVEL_OF_DETAIL)
            {
                for (int k = 0; k < 2; ++k)
                {
                    indices.push_back(j + size * (i + k * LEVEL_OF_DETAIL));
                }
            }
        }

        return Model::Assets::Mesh(vertices, indices);
    }
}

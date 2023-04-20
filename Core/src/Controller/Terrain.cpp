#include "Terrain.hpp"

#include <Controller/AssetLoader/FileLoader.hpp>
#include <Controller/Logger.hpp>

#include <cstdlib>

namespace Vakol::Controller 
{
    Terrain::Terrain(const std::string& path) 
    {
        this->m_model = std::make_shared<Model::Assets::Model>(LoadHeightMap(LoadImage(path, this->m_size, this->m_size)));
    }

    Terrain::Terrain(const int size) 
        : m_size(size), m_model(std::make_shared<Model::Assets::Model>(LoadCLODTerrain(size))) {};

    Terrain::Terrain(const int size, const int iterations, const float filter, const bool random, const int minHeight, const int maxHeight)
        : m_size(size), m_model(std::make_shared<Model::Assets::Model>(LoadFaultFormation(size, iterations, filter, random, minHeight, maxHeight))) {};

    const Model::Assets::Mesh Terrain::LoadHeightMap(unsigned char* data) 
    {
        std::vector<Vertex> vertices;

        const int size = m_size;
        vertices.reserve(size * size);  // allocate memory to reduce number of allocation calls in push_back

        for (int z = 0; z < size; ++z) {
            for (int x = 0; x < size; ++x) {
                const auto pixel_offset = data + (z * size + x);
                const auto y = pixel_offset[0];

                vertices.push_back({
                    glm::vec3((-size / 2.0f + size * x / static_cast<float>(size)) * 1.0f, (y * 0.1f - 16.0f),
                              (-size / 2.0f + size * z / static_cast<float>(size)) * 1.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f),  // normal
                    glm::vec2((x / static_cast<float>(size)), (z / static_cast<float>(size))),
                    glm::vec3(0.0f),  // tangent
                    glm::vec3(0.0f)   // bitangent
                });
            }
        }

        delete[] data;
        data = nullptr;  // don't need the data anymore

        std::vector<unsigned int> indices;

        const int LEVEL_OF_DETAIL = 1;

        indices.reserve((size - 1) * size * 2);

        for (int i = 0; i < size - 1; i += LEVEL_OF_DETAIL) {
            for (int j = 0; j < size; j += LEVEL_OF_DETAIL) {
                for (int k = 0; k < 2; ++k) {
                    indices.push_back(j + size * (i + k * LEVEL_OF_DETAIL));
                }
            }
        }

        return Model::Assets::Mesh(vertices, indices);
    }

    const Model::Assets::Mesh Terrain::LoadFaultFormation(const int size, const int iterations, const float filter, const bool random, const int minHeight, const int maxHeight) 
    {
        Point p1, p2;

        int displacement;

        m_heightMap.resize(size * size);

        m_minHeight = minHeight;
        m_maxHeight = maxHeight;

        if (random) srand(static_cast<unsigned int>(time(NULL)));

        for (int itr = 0; itr < iterations; ++itr) {
            // displacement = this->m_maxHeight - ((this->m_maxHeight - this->m_minHeight) * itr) / iterations;
            displacement = m_maxHeight - ((m_maxHeight - m_minHeight) * itr) / iterations;

            GenRandomPoints(p1, p2, size);

            for (int z = 0; z < size; ++z) {
                for (int x = 0; x < size; ++x) {
                    int crossProd = (x - p1.x) * (p2.z - p1.z) - (p2.x - p1.x) * (z - p1.z);

                    if (crossProd > 0) {
                        m_heightMap[z * size + x] += static_cast<float>(displacement);
                    }
                }
            }
        }

        ApplyFIRFilter(m_heightMap, size, filter);
        NormalizeValues(m_heightMap, size);

        std::vector<Vertex> vertices;

        vertices.reserve(size * size);  // allocate memory to reduce number of allocation calls in push_back

        for (int z = 0; z < size; ++z) {
            for (int x = 0; x < size; ++x) {
                unsigned char y = static_cast<unsigned char>(m_heightMap.at(z * size + x));

                vertices.push_back({
                    glm::vec3((-size / 2.0f + size * x / static_cast<float>(size)) * 1.0f, (y * 0.4f - 16.0f),
                              (-size / 2.0f + size * z / static_cast<float>(size)) * 1.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f),  // normal
                    glm::vec2((x / static_cast<float>(size)), (z / static_cast<float>(size))),
                    glm::vec3(0.0f),  // tangent
                    glm::vec3(0.0f)   // bitangent
                });
            }
        }

        std::vector<unsigned int> indices;

        indices.reserve((size - 1) * size * 2);

        const int LEVEL_OF_DETAIL = 1;

        for (int i = 0; i < size - 1; i += LEVEL_OF_DETAIL) {
            for (int j = 0; j < size; j += LEVEL_OF_DETAIL) {
                for (int k = 0; k < 2; ++k) {
                    indices.push_back(j + size * (i + k * LEVEL_OF_DETAIL));
                }
            }
        }

        return Model::Assets::Mesh(vertices, indices);
    }

    void Terrain::GenRandomPoints(Point& p1, Point& p2, const int size) {
        p1.x = rand() % size;
        p1.z = rand() % size;

        int index = 0;  // index is here to prevent infinite loop (too lazy to implement assert tho)

        do {
            p2.x = rand() % size;
            p2.z = rand() % size;

            if (index > 1000) {
                VK_WARN("Failed to generate two random points.");
                VK_WARN("Breaking to prevent infinite loop");
            }

            ++index;
        } while (p1 == p2);
    }

    void Terrain::ApplyFIRFilter(std::vector<float>& arr, const int size, const float filter) {
        // left to right
        for (int z = 0; z < size; ++z) {
            float prev = arr.at(z * size + 0);

            for (int x = 1; x < size; ++x) prev = FIRSinglePass(arr, z * size + x, prev, filter);
        }

        // right to left
        for (int z = 0; z < size; ++z) {
            float prev = arr.at(z * size + size - 1);

            for (int x = size - 2; x >= 0; --x) prev = FIRSinglePass(arr, z * size + x, prev, filter);
        }

        // bottom to top
        for (int x = 0; x < size; ++x) {
            float prev = arr.at(x * size + 0);

            for (int z = 1; z < size; ++z) prev = FIRSinglePass(arr, x * size + z, prev, filter);
        }

        // top to bottom
        for (int x = 0; x < size; ++x) {
            float prev = arr.at(x * size + (size - 1));

            for (int z = size - 2; z >= 0; --z) prev = FIRSinglePass(arr, x * size + z, prev, filter);
        }
    }

    float Terrain::FIRSinglePass(std::vector<float>& arr, const int index, const float prev, float filter) {
        auto cur_val = arr.at(index);
        auto new_val = filter * prev + (1 - filter) * cur_val;

        arr[index] = new_val;

        return new_val;
    }

    void Terrain::NormalizeValues(std::vector<float>& arr, const int size) 
    {
        float min = arr.at(0);
        float max = arr.at(0);

        float height;

        // find min and max of height values
        for (int i = 1; i < size * size; ++i) {
            if (arr.at(i) > max)
                max = arr[i];
            else if (arr.at(i) < min)
                min = arr[i];
        }

        // find range of the altitude
        if (max <= min) return;

        height = max - min;

        // scale values between 0-255
        for (int i = 0; i < size * size; ++i) arr[i] = ((arr.at(i) - min) / height) * 255.0f;
    }

    const Model::Assets::Mesh Terrain::LoadCLODTerrain(const int size)
    {
        std::vector<float> vertices;

        unsigned int patch_size = (3 * sizeof(float) + 2 * sizeof(float)); // position (3 floats) + uv (2 floats) = 20 bytes

        vertices.reserve(patch_size * patch_size * patch_size);

        for(unsigned int i = 0; i <= patch_size - 1; i++)
        {
            for(unsigned int j = 0; j <= patch_size - 1; j++)
            {
                vertices.push_back(-size / 2.0f + size * j / (float)patch_size); // v.z
                vertices.push_back(0.0f); // v.y
                vertices.push_back(-size / 2.0f + size * i / (float)patch_size); // v.x
                vertices.push_back(i / (float)patch_size); // u
                vertices.push_back(j / (float)patch_size); // v

                vertices.push_back(-size / 2.0f + size * j / (float)patch_size); // v.z
                vertices.push_back(0.0f); // v.y
                vertices.push_back(-size / 2.0f + size * (i + 1) / (float)patch_size); // v.x
                vertices.push_back((i + 1) / (float)patch_size); // u
                vertices.push_back(j / (float)patch_size); // v

                vertices.push_back(-size / 2.0f + size * (j + 1) / (float)patch_size); // v.z
                vertices.push_back(0.0f); // v.y
                vertices.push_back(-size / 2.0f + size * i / (float)patch_size); // v.x
                vertices.push_back(i / (float)patch_size); // u
                vertices.push_back((j + 1) / (float)patch_size); // v

                vertices.push_back(-size / 2.0f + size * (j + 1) / (float)patch_size); // v.z
                vertices.push_back(0.0f); // v.y
                vertices.push_back(-size / 2.0f + size * (i + 1) / (float)patch_size); // v.x
                vertices.push_back((i + 1) / (float)patch_size); // u
                vertices.push_back((j + 1) / (float)patch_size); // v
            }
        }

        VK_TRACE("Loaded {0} patches of 4 control points each", patch_size * patch_size);
        VK_TRACE("Processing {0} vertices in vertex shader", patch_size * patch_size * patch_size);

        return {vertices};
    }

    float Terrain::GetHeight(float x, float z) const 
    {
        // get the height of the terrain at a given x and z coordinate
        // this is done by interpolating the height of the 4 vertices that surround the point
        // the height is then interpolated between the 4 vertices

        if (x < 0 || x >= m_size || z < 0 || z >= m_size) 
            return 0.0f;

        // get the 4 vertices that surround the point
        int x0 = static_cast<int>(x) % m_size;
        int x1 = (x0 + 1) % m_size;
        int z0 = static_cast<int>(z) % m_size;
        int z1 = (z0 + 1) % m_size;

        // Handle negative x and z values by wrapping them around
        if (x0 < 0) x0 += m_size;
        if (x1 < 0) x1 += m_size;
        if (z0 < 0) z0 += m_size;
        if (z1 < 0) z1 += m_size;

        // get the heights of the 4 vertices
        auto vertices = m_model->GetMesh().GetVertexArray()->GetVertices();

        float y0 = vertices[z0 * m_size + x0].position.y;
        float y1 = vertices[z0 * m_size + x1].position.y;
        float y2 = vertices[z1 * m_size + x0].position.y;
        float y3 = vertices[z1 * m_size + x1].position.y;

        // get the distance between the point and the 4 vertices
        float dx1 = x - x0;
        float dx2 = x1 - x;
        float dz1 = z - z0;
        float dz2 = z1 - z;

        float w1 = dx2 * dz2;
        float w2 = dx1 * dz2;
        float w3 = dx2 * dz1;
        float w4 = dx1 * dz1;

        // calculate the interpolated height
        float y = (y0 * w1 + y1 * w2 + y2 * w3 + y3 * w4) / (w1 + w2 + w3 + w4);

        return y;
    }
}  // namespace Vakol::Controller

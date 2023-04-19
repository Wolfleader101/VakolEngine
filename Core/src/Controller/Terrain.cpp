#include "Terrain.hpp"

#include <Controller/Logger.hpp>
#include <Controller/AssetLoader/FileLoader.hpp>

#include <cstdlib>

struct Point
{
    int x = 0;
    int z = 0;

    const bool operator==(const Point& other) const
    {
        return this->x == other.x && this->z == other.z;
    }

    const Point operator-(const Point& other) const
    {
        return {this->x - other.x, this->z - other.z}; 
    }
};

void GenRandomPoints(Point& p1, Point& p2, const int size);
void NormalizeValues(std::vector<float>& arr, const int size);

void ApplyFIRFilter(std::vector<float>& arr, const int size, const float filter);
float FIRSinglePass(std::vector<float>& arr, const int index, const float prev, const float filter);

namespace Vakol::Controller
{
    Terrain::Terrain(const std::string& path)
    {
        this->m_terrain = LoadHeightMap(LoadImage(path, this->m_size, this->m_size));
    }

    const Model::Assets::Mesh Terrain::LoadHeightMap(unsigned char* data)
    {
        std::vector<Vertex> vertices;

        const int size = m_size;
        vertices.reserve(size * size); // allocate memory to reduce number of allocation calls in push_back

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
                    glm::vec3(0.0f, 1.0f, 0.0f), // normal
                    glm::vec2((x / static_cast<float>(size)),
                              (z / static_cast<float>(size))),
                    glm::vec3(0.0f), // tangent
                    glm::vec3(0.0f)  // bitangent
                });
            }
        }
        
        delete [] data;
        data = nullptr; // don't need the data anymore

        std::vector<unsigned int> indices;

        const int LEVEL_OF_DETAIL = 1;

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

    const Model::Assets::Mesh Terrain::LoadFaultFormation(const int size, const int iterations, const float filter, const bool random)
    {

        Point p1, p2;

        int displacement;

        m_heightMap.resize(size * size);

        m_minHeight = -256;
        m_maxHeight = 256;

        if (random)
            srand(static_cast<unsigned int>(time(NULL)));
            
        for (int itr = 0; itr < iterations; ++itr)
        {
            //displacement = this->m_maxHeight - ((this->m_maxHeight - this->m_minHeight) * itr) / iterations;
            displacement = m_maxHeight - ((m_maxHeight - m_minHeight) * itr) / iterations;

            ::GenRandomPoints(p1, p2, size);

            for (int z = 0; z < size; ++z)
            {
                for (int x = 0; x < size; ++x)
                {
                    int crossProd = (x - p1.x) * (p2.z - p1.z) - (p2.x - p1.x) * (z - p1.z);

                    if (crossProd > 0)
                    {
                        m_heightMap[z * size + x] += static_cast<float>(displacement);
                    }
                }
            }
        }

        ::ApplyFIRFilter(m_heightMap, size, filter);
        ::NormalizeValues(m_heightMap, size);

        std::vector<Vertex> vertices;

        vertices.reserve(size * size); // allocate memory to reduce number of allocation calls in push_back

        for (int z = 0; z < size; ++z)
        {
            for (int x = 0; x < size; ++x)
            {
                unsigned char y = static_cast<unsigned char>(m_heightMap.at(z * size + x));

                vertices.push_back({
                    glm::vec3((-size / 2.0f + size * x / static_cast<float>(size)) * 1.0f,
                              (y * 0.25f - 16.0f),
                              (-size / 2.0f + size * z / static_cast<float>(size)) * 1.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f), // normal
                    glm::vec2((x / static_cast<float>(size)),
                              (z / static_cast<float>(size))),
                    glm::vec3(0.0f), // tangent
                    glm::vec3(0.0f)  // bitangent
                });
            }
        }

        std::vector<unsigned int> indices;

        indices.reserve((size - 1) * size * 2);

        const int LEVEL_OF_DETAIL = 1;

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

void GenRandomPoints(Point& p1, Point& p2, const int size)
{
    p1.x = rand() % size;
    p1.z = rand() % size;

    int index = 0; // index is here to prevent infinite loop (too lazy to implement assert tho)

    do
    {
        p2.x = rand() % size;
        p2.z = rand() % size;

        if (index > 1000)
        {
            VK_WARN("Failed to generate two random points.");
            VK_WARN("Breaking to prevent infinite loop");
        }

        ++index;
    } while (p1 == p2);
}

void ApplyFIRFilter(std::vector<float>& arr, const int size, const float filter)
{
    // left to right
    for (int z = 0; z < size; ++z)
    {
        float prev = arr.at(z * size + 0);
        
        for (int x = 1; x < size; ++x)
            prev = FIRSinglePass(arr, z * size + x, prev, filter);
    }

    // right to left
    for (int z = 0; z < size; ++z)
    {
        float prev = arr.at(z * size + size - 1);

        for (int x = size - 2; x >= 0; --x)
            prev = FIRSinglePass(arr, z * size + x, prev, filter);
    }

    // bottom to top
    for (int x = 0; x < size; ++x)
    {
        float prev = arr.at(x * size + 0);

        for (int z = 1; z < size; ++z)
            prev = FIRSinglePass(arr, x * size + z, prev, filter);
    }

    // top to bottom
    for (int x = 0; x < size; ++x)
    {
        float prev = arr.at(x * size + (size - 1));

        for (int z = size - 2; z >= 0; --z)
            prev = FIRSinglePass(arr, x * size + z, prev, filter);
    }
}

float FIRSinglePass(std::vector<float>& arr, const int index, const float prev, float filter)
{
    auto cur_val = arr.at(index);
    auto new_val = filter * prev + (1 - filter) * cur_val;

    arr[index] = new_val;

    return new_val;
}

void NormalizeValues(std::vector<float>& arr, const int size)
{
    float min = arr.at(0);
    float max = arr.at(0);

    float height;

    // find min and max of height values
    for (int i = 1; i < size * size; ++i)
    {
        if (arr.at(i) > max)
            max = arr[i];
        else if (arr.at(i) < min)
            min = arr[i];
    }

    // find range of the altitude
    if (max <= min) return;

    height = max - min;

    // scale values between 0-255
    for (int i = 0; i < size * size; ++i)
        arr[i] = ((arr.at(i) - min) / height) * 255.0f;
}
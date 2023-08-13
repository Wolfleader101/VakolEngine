#include "Terrain.hpp"

#include <Controller/AssetLoader/FileLoader.hpp>
#include <Controller/Logger.hpp>
#include <algorithm>
#include <cstdlib>
#include <iostream>

namespace Vakol::Controller
{
    Terrain LoadHeightMapTerrain(std::string&& path, const float min, const float max)
    {
        Terrain terrain{};

        terrain.SetMinMax(min, max);

        int size;
        const auto data = LoadImage(std::move(path), size, size);

        terrain.SetSize(size);
        terrain.SetData(data);

        terrain.SetModel(terrain.load_height_map_mesh());

        return terrain;
    }

    Terrain LoadCLODTerrain(std::string&& path, const float min, const float max)
    {
        Terrain terrain{};

        terrain.SetMinMax(min, max);

        int size;
        const auto data = LoadImage(std::move(path), size, size);

        terrain.SetSize(size);
        terrain.SetData(data);

        terrain.SetModel(terrain.load_clod_terrain_mesh());

        return terrain;
    }

    void Terrain::SetData(const unsigned char* data)
    {
        const auto size = static_cast<size_t>(this->m_size);

        this->m_data.reserve(size * size);

        for (int x = 0; x < this->m_size; ++x)
            for (int z = 0; z < this->m_size; ++z)
                this->m_data.push_back(data[x * m_size + z]);

        SetHeightMap(); // used for physics. Need floats. Is duplicating but who is that hurting

        delete[] data;
        data = nullptr;
    }

    Model::Assets::Mesh Terrain::load_height_map_mesh() const
    {
        std::vector<float> vertices;

        const auto size = static_cast<size_t>(m_size);
        vertices.reserve(size * size);
        // allocate memory to reduce number of allocation calls in push_back

        for (int z = 0; z < m_size; ++z)
        {
            for (int x = 0; x < m_size; ++x)
            {
                const auto pixel_offset = m_height_map.data() + (z * m_size + x);
                const auto y = pixel_offset[0] - (m_max_height - m_min_height) / 2.0f;

                vertices.push_back(-m_size / 2.0f + m_size * x / static_cast<float>(m_size));
                vertices.push_back(y);
                vertices.push_back(-m_size / 2.0f + m_size * z / static_cast<float>(m_size));
                vertices.push_back(x / static_cast<float>(m_size));
                vertices.push_back(z / static_cast<float>(m_size));
            }
        }

        std::vector<unsigned int> indices;

        constexpr int lod = 1;

        indices.reserve((size - 1) * size * 2);

        for (int i = 0; i < m_size - 1; i += lod)
        {
            for (int j = 0; j < m_size; j += lod)
            {
                for (int k = 0; k < 2; ++k)
                {
                    indices.push_back(j + m_size * (i + k * lod));
                }
            }
        }

        return {vertices, indices, 5 * sizeof(float)};
    }

    Model::Assets::Mesh Terrain::load_fault_formation_mesh(const int size, const int iterations, const float filter,
                                                           const bool random, const int minHeight, const int maxHeight)
    {
        Point p1, p2;

        m_height_map.resize(static_cast<size_t>(size) * static_cast<size_t>(size));

        m_min_height = static_cast<float>(minHeight);
        m_max_height = static_cast<float>(maxHeight);

        if (random)
            srand(static_cast<unsigned int>(time(nullptr)));

        for (int itr = 0; itr < iterations; ++itr)
        {
            const int displacement = static_cast<int>(m_max_height - (m_max_height - m_min_height) * itr / iterations);

            GenRandomPoints(p1, p2, size);

            for (int z = 0; z < size; ++z)
            {
                for (int x = 0; x < size; ++x)
                {
                    if (const int cross_product = (x - p1.x) * (p2.z - p1.z) - (p2.x - p1.x) * (z - p1.z);
                        cross_product > 0)
                    {
                        m_height_map[z * size + x] += static_cast<float>(displacement);
                    }
                }
            }
        }

        ApplyFIRFilter(m_height_map, size, filter);
        NormalizeValues(m_height_map, size);

        std::vector<float> vertices;

        vertices.reserve(
            static_cast<size_t>(size) *
            static_cast<size_t>(size)); // allocate memory to reduce number of allocation calls in push_back

        for (int z = 0; z < size; ++z)
        {
            for (int x = 0; x < size; ++x)
            {
                const auto y = static_cast<unsigned char>(m_height_map.at(z * size + x));

                vertices.push_back((-size / 2.0f + size * x / static_cast<float>(size)) * 1.0f);
                vertices.push_back(y * 0.4f - 16.0f);
                vertices.push_back((-size / 2.0f + size * z / static_cast<float>(size)) * 1.0f);

                vertices.push_back(x / static_cast<float>(size));
                vertices.push_back(z / static_cast<float>(size));
            }
        }

        std::vector<unsigned int> indices;

        indices.reserve((size - 1) * size * 2);

        constexpr int lod = 1;

        for (int i = 0; i < size - 1; i += lod)
        {
            for (int j = 0; j < size; j += lod)
            {
                for (int k = 0; k < 2; ++k)
                {
                    indices.push_back(j + size * (i + k * lod));
                }
            }
        }

        return {vertices, indices, 5 * sizeof(float)};
    }

    Model::Assets::Mesh Terrain::load_clod_terrain_mesh() const
    {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        constexpr auto patch_size = 5 * sizeof(float);

        constexpr auto size = static_cast<int>(patch_size);

        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
            {
                for (int k = 0; k < Model::NUM_PATCH_PTS; ++k)
                {
                    const auto x =
                        -m_size / 2.0f + m_size * static_cast<float>(i * 3 + k % 2) / static_cast<float>(size);
                    const auto z =
                        -m_size / 2.0f + m_size * static_cast<float>(j * 3 + k / 2) / static_cast<float>(size);

                    VK_TRACE("X: {0}, Z: {1} | i = {2}", x, z, k);

                    const auto u = i / (static_cast<float>(size) * 3.0f);
                    const auto v = j / (static_cast<float>(size) * 3.0f);

                    VK_TRACE("U: {0}, V: {1}", u, v);

                    if (k == 3)
                        std::cout << std::endl;
                }
            }
        }

        return {vertices, indices, patch_size};
    }

    void Terrain::GenRandomPoints(Point& p1, Point& p2, const int size)
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

    void Terrain::ApplyFIRFilter(std::vector<float>& arr, const int size, const float filter)
    {
        // left to right
        for (int z = 0; z < size; ++z)
        {
            float prev = arr.at(z * size + 0);

            for (int x = 1; x < size; ++x)
                prev = FirSinglePass(arr, z * size + x, prev, filter);
        }

        // right to left
        for (int z = 0; z < size; ++z)
        {
            float prev = arr.at(z * size + size - 1);

            for (int x = size - 2; x >= 0; --x)
                prev = FirSinglePass(arr, z * size + x, prev, filter);
        }

        // bottom to top
        for (int x = 0; x < size; ++x)
        {
            float prev = arr.at(x * size + 0);

            for (int z = 1; z < size; ++z)
                prev = FirSinglePass(arr, x * size + z, prev, filter);
        }

        // top to bottom
        for (int x = 0; x < size; ++x)
        {
            float prev = arr.at(x * size + (size - 1));

            for (int z = size - 2; z >= 0; --z)
                prev = FirSinglePass(arr, x * size + z, prev, filter);
        }
    }

    float Terrain::FirSinglePass(std::vector<float>& arr, const int index, const float prev, const float filter)
    {
        const auto cur_val = arr.at(index);
        const auto new_val = filter * prev + (1 - filter) * cur_val;

        arr[index] = new_val;

        return new_val;
    }

    void Terrain::NormalizeValues(std::vector<float>& arr, const int size)
    {
        float Oldmin = std::min_element(arr.begin(), arr.end())[0];
        float Oldmax = std::max_element(arr.begin(), arr.end())[0];
        // find range of the altitude
        if (Oldmax <= Oldmin)
            return;
        float oldRange = Oldmax - Oldmin;
        float newRange = m_max_height - m_min_height;
        for (int i = 0; i < size * size; ++i)
        {
            arr[i] = (((arr.at(i) - Oldmin) * newRange) / oldRange) + m_min_height;
        }
    }

    float Terrain::GetHeight(float x, float z) const
    {
        // Shift x and z to the range [0, m_size)
        x = (x + m_size / 2.0f);
        z = (z + m_size / 2.0f);

        // Clamp x and z to the terrain's dimensions
        x = std::clamp(x, 0.0f, m_size - 1.0f);
        z = std::clamp(z, 0.0f, m_size - 1.0f);

        // Calculate the height map indices using the coordinates
        int xInt = static_cast<int>(x);
        int zInt = static_cast<int>(z);

        // Calculate the fractions
        float fracX = x - xInt;
        float fracZ = z - zInt;

        // Indices of the four corners of the cell
        int x0 = std::clamp(xInt, 0, m_size - 2);
        int z0 = std::clamp(zInt, 0, m_size - 2);
        int x1 = x0 + 1;
        int z1 = z0 + 1;

        // Heights at the four corners
        auto& vertices = m_model->mesh().vertices();
        float h00 = vertices[(z0 * m_size + x0)].position.y;
        float h10 = vertices[(z0 * m_size + x1)].position.y;
        float h01 = vertices[(z1 * m_size + x0)].position.y;
        float h11 = vertices[(z1 * m_size + x1)].position.y;

        // Bilinear interpolation
        float height = h00 * (1 - fracX) * (1 - fracZ) + h10 * fracX * (1 - fracZ) + h01 * (1 - fracX) * fracZ +
                       h11 * fracX * fracZ;

        return height;
    }

    void Terrain::SetHeightMap()
    {
        for (float value : m_data)
            m_height_map.push_back(value);

        NormalizeValues(m_height_map, m_size);
    }

} // namespace Vakol::Controller

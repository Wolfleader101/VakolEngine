#include "Terrain.hpp"

#include <Controller/AssetLoader/FileLoader.hpp>
#include <Controller/Logger.hpp>
#include <algorithm>
#include <cstdlib>

namespace Vakol::Controller
{
    Terrain LoadHeightMapTerrain(std::string&& path, float min, float max)
    {
        Terrain terrain {};

        terrain.SetMinMax({ min, max });

        int size;
        const auto data = LoadImage(std::move(path), size, size);

        terrain.SetSize(size);
        terrain.SetData(data);

        terrain.SetModel(terrain.load_height_map_mesh());

        

        return terrain;
    }

    Terrain LoadCLODTerrain(std::string&& path)
    {
        Terrain terrain {};

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

        SetHeightMap();
        delete[] data;
        data = nullptr;

    }

	Model::Assets::Mesh Terrain::load_height_map_mesh() const
	{
        std::vector<float> vertices;

        const auto size = static_cast<size_t>(m_size);
        vertices.reserve(size * size);
        // allocate memory to reduce number of allocation calls in push_back

        for (int z = 0; z < m_size; ++z) {
            for (int x = 0; x < m_size; ++x) 
            {
                const auto pixel_offset = m_height_map.data() + (z * m_size + x);
                const auto y = pixel_offset[0] - (m_max_height - m_min_height) / 2.0f;

                vertices.push_back((-m_size / 2.0f + m_size * x / static_cast<float>(m_size)) * 1.0f); 
                vertices.push_back(y);
                vertices.push_back((-m_size / 2.0f + m_size * z / static_cast<float>(m_size)) * 1.0f);
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
                for (int k = 0; k < 2; ++k) {
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

        m_min_height = minHeight;
        m_max_height = maxHeight;

        if (random) srand(static_cast<unsigned int>(time(nullptr)));

        for (int itr = 0; itr < iterations; ++itr)
        {
            const int displacement = m_max_height - (m_max_height - m_min_height) * itr / iterations;

            GenRandomPoints(p1, p2, size);

            for (int z = 0; z < size; ++z) {
                for (int x = 0; x < size; ++x) {
	                if (const int cross_product = (x - p1.x) * (p2.z - p1.z) - (p2.x - p1.x) * (z - p1.z); cross_product > 0) {
                        m_height_map[z * size + x] += static_cast<float>(displacement);
                    }
                }
            }
        }

        ApplyFIRFilter(m_height_map, size, filter);
        NormalizeValues(m_height_map, size);

        std::vector<float> vertices;

        vertices.reserve(static_cast<size_t>(size) * static_cast<size_t>(size));  // allocate memory to reduce number of allocation calls in push_back

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

        for (int i = 0; i < size - 1; i += lod) {
            for (int j = 0; j < size; j += lod) {
                for (int k = 0; k < 2; ++k) {
                    indices.push_back(j + size * (i + k * lod));
                }
            }
        }

        return {vertices, indices, 5 * sizeof(float)};
    }

	Model::Assets::Mesh Terrain::load_clod_terrain_mesh() const
    {
        std::vector<float> vertices;

        constexpr auto patch_size = 5 * sizeof(float);  // position (3 floats) + uv (2 floats) = 20 bytes

        vertices.reserve(patch_size * patch_size * patch_size);

        constexpr auto num_patches = static_cast<int>(patch_size);

        for (int i = 0; i <= num_patches - 1; i++)
        {
            for (int j = 0; j <= num_patches - 1; j++)
            {
                vertices.push_back(-m_size / 2.0f + m_size * j / static_cast<float>(num_patches));  // v.z
                vertices.push_back(0.0f);                                         // v.y
                vertices.push_back(-m_size / 2.0f + m_size * i / static_cast<float>(num_patches));  // v.x
                vertices.push_back(i / static_cast<float>(num_patches));                        // u
                vertices.push_back(j / static_cast<float>(num_patches));                        // v

                vertices.push_back(-m_size / 2.0f + m_size * j / static_cast<float>(num_patches));        // v.z
                vertices.push_back(0.0f);                                               // v.y
                vertices.push_back(-m_size / 2.0f + m_size * (i + 1) / static_cast<float>(num_patches));  // v.x
                vertices.push_back((i + 1) / static_cast<float>(num_patches));                        // u
                vertices.push_back(j / static_cast<float>(num_patches));                              // v

                vertices.push_back(-m_size / 2.0f + m_size * (j + 1) / static_cast<float>(num_patches));  // v.z
                vertices.push_back(0.0f);                                               // v.y
                vertices.push_back(-m_size / 2.0f + m_size * i / static_cast<float>(num_patches));        // v.x
                vertices.push_back(i / static_cast<float>(num_patches));                              // u
                vertices.push_back((j + 1) / static_cast<float>(num_patches));                        // v

                vertices.push_back(-m_size / 2.0f + m_size * (j + 1) / static_cast<float>(num_patches));  // v.z
                vertices.push_back(0.0f);                                               // v.y
                vertices.push_back(-m_size / 2.0f + m_size * (i + 1) / static_cast<float>(num_patches));  // v.x
                vertices.push_back((i + 1) / static_cast<float>(num_patches));                        // u
                vertices.push_back((j + 1) / static_cast<float>(num_patches));                        // v
            }
        }

        std::vector<unsigned int> indices;

        return { vertices, indices, num_patches };
    }

    void Terrain::GenRandomPoints(Point& p1, Point& p2, const int size)
	{
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

    void Terrain::ApplyFIRFilter(std::vector<float>& arr, const int size, const float filter)
	{
        // left to right
        for (int z = 0; z < size; ++z) {
            float prev = arr.at(z * size + 0);

            for (int x = 1; x < size; ++x) prev = FirSinglePass(arr, z * size + x, prev, filter);
        }

        // right to left
        for (int z = 0; z < size; ++z) {
            float prev = arr.at(z * size + size - 1);

            for (int x = size - 2; x >= 0; --x) prev = FirSinglePass(arr, z * size + x, prev, filter);
        }

        // bottom to top
        for (int x = 0; x < size; ++x) {
            float prev = arr.at(x * size + 0);

            for (int z = 1; z < size; ++z) prev = FirSinglePass(arr, x * size + z, prev, filter);
        }

        // top to bottom
        for (int x = 0; x < size; ++x) {
            float prev = arr.at(x * size + (size - 1));

            for (int z = size - 2; z >= 0; --z) prev = FirSinglePass(arr, x * size + z, prev, filter);
        }
    }

    float Terrain::FirSinglePass(std::vector<float>& arr, const int index, const float prev, const float filter)
	{
		const auto cur_val = arr.at(index);
		const auto new_val = filter * prev + (1 - filter) * cur_val;

        arr[index] = new_val;

        return new_val;
    }

   
    void Terrain::NormalizeValues(std::vector<float>&arr, const int size)
    {
        float Oldmin = std::min_element(arr.begin(), arr.end())[0];
        float Oldmax = std::max_element(arr.begin(), arr.end())[0];
        // find range of the altitude
        if (Oldmax <= Oldmin) return;
        float oldRange = Oldmax - Oldmin;
        float newRange = m_max_height - m_min_height;
        float height = Oldmax - Oldmin;
        for (int i = 0; i < size * size; ++i) {
            arr[i] = (((arr.at(i) - Oldmin) * newRange) / oldRange) + m_min_height;
        }
    }
    

	float Terrain::GetHeight(float x, float z) const 
    {
        // Clamp x and z to the terrain's dimensions
        x = std::clamp(x, -m_size / 2.0f, m_size / 2.0f - m_size / (m_size - 1));
        z = std::clamp(z, -m_size / 2.0f, m_size / 2.0f - m_size / (m_size - 1));

        // Normalize x and z to the range [0, 1]
        const float norm_x = (x + m_size / 2.0f) / m_size;
        const float norm_z = (z + m_size / 2.0f) / m_size;

        // Calculate the height map indices using the normalized coordinates
        const auto i = static_cast<unsigned int>(norm_x * (m_size - 1));
        const auto j = static_cast<unsigned int>(norm_z * (m_size - 1));

        // Get the height at the corresponding height map index
        const float height = m_height_map[std::min(static_cast<int>(i), m_size - 1) * m_size + std::min(static_cast<int>(j), m_size - 1)];

        return height;
    }

    void Terrain::SetHeightMap()
    {
	    for(float value : m_data)
			m_height_map.push_back(value);

        NormalizeValues(m_height_map, m_size);

	    
    }

}

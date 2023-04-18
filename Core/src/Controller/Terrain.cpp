#include "Terrain.hpp"

#include "Logger.hpp"

#include "Model/Components.hpp"
#include "AssetLoader/FileLoader.hpp"

namespace Vakol::Controller 
{
    Terrain::Terrain(Entity& entity) : m_entity(entity) 
    {
        m_entity.AddComponent<Components::Drawable>();
    };

    void Terrain::LoadFaultFormation(unsigned int terrainSize) 
    {
        m_terrainSize = terrainSize;
        m_heightMap = std::vector<std::vector<float>>(m_terrainSize, std::vector<float>(m_terrainSize));

        float deltaHeight = m_maxHeight - m_minHeight;

        for (int i = 0; i < m_iterations; i++) 
        {
            float ratio = static_cast<float>(i) / static_cast<float>(m_iterations);
            float height = m_maxHeight - (ratio * deltaHeight);

            glm::vec3 randomP1 = glm::vec3(rand() % m_terrainSize, 0, rand() % m_terrainSize);
            glm::vec3 randomP2 = glm::vec3(rand() % m_terrainSize, 0, rand() % m_terrainSize);

            while (randomP1 == randomP2) {
                randomP2 = glm::vec3(rand() % m_terrainSize, 0, rand() % m_terrainSize);
            }

            // calculate the direction of the line
            glm::vec3 lineDir = randomP2 - randomP1;

            for (int z = 0; z < m_terrainSize; z++)
            {
                for (int x = 0; x < m_terrainSize; x++) 
                {
                    glm::vec3 in(lineDir);
                    in.x = x - randomP1.x;
                    in.z = z - randomP1.z;

                    float cross = in.x * lineDir.z - in.z * lineDir.x;

                    if (cross > 0) m_heightMap[z][x] += height;
                }
            }
        }

        // apply FIR filters

        // left to right
        for (int z = 0; z < m_terrainSize; z++) 
        {
            float prev = m_heightMap[0][z];

            for (int x = 1; x < m_terrainSize; x++) 
            {
                float cur = m_heightMap[x][z];
                float newVal = m_firFilter * prev + (1.0f - m_firFilter) * cur;

                m_heightMap[x][z] = newVal;
                prev = newVal;
            }
        }

        // right to left
        for (int z = 0; z < m_terrainSize; z++) 
        {
            float prev = m_heightMap[m_terrainSize - 1][z];
            for (int x = m_terrainSize - 2; x >= 0; x--) 
            {
                float cur = m_heightMap[x][z];
                float newVal = m_firFilter * prev + (1.0f - m_firFilter) * cur;

                m_heightMap[x][z] = newVal;
                prev = newVal;
            }
        }

        // bottom to top
        for (int x = 0; x < m_terrainSize; x++) 
        {
            float prev = m_heightMap[x][0];

            for (int z = 1; z < m_terrainSize; z++) 
            {
                float cur = m_heightMap[x][z];
                float newVal = m_firFilter * prev + (1.0f - m_firFilter) * cur;

                m_heightMap[x][z] = newVal;
                prev = newVal;
            }
        }

        // top to bottom
        for (int x = 0; x < m_terrainSize; x++) 
        {
            float prev = m_heightMap[x][m_terrainSize - 1];
            for (int z = m_terrainSize - 2; z >= 0; z--) 
            {
                float cur = m_heightMap[x][z];
                float newVal = m_firFilter * prev + (1.0f - m_firFilter) * cur;

                m_heightMap[x][z] = newVal;
                prev = newVal;
            }
        }

        // normalise the heightmap
        float min = m_heightMap[0][0];
        float max = min;

        for (int i = 1; i < m_terrainSize * m_terrainSize; i++) 
        {
            if (m_heightMap[i / m_terrainSize][i % m_terrainSize] < min)
                min = m_heightMap[i / m_terrainSize][i % m_terrainSize];

            if (m_heightMap[i / m_terrainSize][i % m_terrainSize] > max)
                max = m_heightMap[i / m_terrainSize][i % m_terrainSize];
        }

        if (max <= min) return;

        float delta = max - min;
        float range = m_maxHeight - m_minHeight;

        for (int i = 0; i < m_terrainSize * m_terrainSize; i++) 
        {
            m_heightMap[i / m_terrainSize][i % m_terrainSize] = ((m_heightMap[i / m_terrainSize][i % m_terrainSize] - min) / delta) * range + m_minHeight;
        }
    }

    void Terrain::Generate() { GenerateDrawable(); }

    void Terrain::GenerateDrawable() 
    {
        InitVertices();
        InitIndices();

        Assets::Mesh mesh(m_vertices, m_indices);

        auto& drawable = m_entity.GetComponent<Components::Drawable>();

        drawable.model_ptr = std::make_shared<Model::Assets::Model>(mesh);
    }

    void Terrain::LoadHeightMap(const std::string& heightMap) 
    {
        int width = 0, height = 0;
        unsigned char* data = LoadImage(heightMap, width, height);

        if (data) 
        {
            m_terrainSize = width | height;

            // initialise a 2d vector
            m_heightMap = std::vector<std::vector<float>>(m_terrainSize, std::vector<float>(m_terrainSize));

            // read the heightmap data into the 2d vector
            for (int z = 0; z < m_terrainSize; z++) {
                for (int x = 0; x < m_terrainSize; x++) {
                    m_heightMap[z][x] = static_cast<float>(data[z * m_terrainSize + x]) * 0.1f;
                }
            }

            delete[] data;
        }
    }

    void Terrain::InitVertices() 
    {
        m_vertices.resize(m_terrainSize * m_terrainSize);

        // generate a grid of vertices
        for (int z = 0; z < m_terrainSize; z++) 
        {
            for (int x = 0; x < m_terrainSize; x++) 
            {
                m_vertices[z * m_terrainSize + x].position = glm::vec3(x, m_heightMap[z][x] * m_yScale - m_yShift, z);

                m_vertices[z * m_terrainSize + x].uv = glm::vec2(x / static_cast<float>(m_terrainSize), z / static_cast<float>(m_terrainSize));
            }
        }
    }

    void Terrain::InitIndices() 
    {
        m_indices.reserve(m_terrainSize * (m_terrainSize - 1) * 2); // allocate x amount of memory in advance (reduces number of allocations) in push_back

        for (int i = 0; i < m_terrainSize - 1; i++)  // for each row a.k.a. each strip
        {
            for (int j = 0; j < m_terrainSize; j++)  // for each column
            {
                for (int k = 0; k < 2; k++)  // for each side of the strip
                {
                    m_indices.push_back(j + m_terrainSize * (i + k));
                }
            }
        }
    }

    float Terrain::GetHeight(float x, float z) 
    {
        // get the height of the terrain at a given x and z coordinate
        // this is done by interpolating the height of the 4 vertices that surround the point
        // the height is then interpolated between the 4 vertices

        if (x < 0 || x >= m_terrainSize || z < 0 || z >= m_terrainSize) {
            return 0.0f;
        }

        // get the 4 vertices that surround the point
        int x0 = static_cast<int>(x) % m_terrainSize;
        int x1 = (x0 + 1) % m_terrainSize;
        int z0 = static_cast<int>(z) % m_terrainSize;
        int z1 = (z0 + 1) % m_terrainSize;

        // Handle negative x and z values by wrapping them around
        if (x0 < 0) x0 += m_terrainSize;
        if (x1 < 0) x1 += m_terrainSize;
        if (z0 < 0) z0 += m_terrainSize;
        if (z1 < 0) z1 += m_terrainSize;

        // get the heights of the 4 vertices
        float y0 = m_vertices[z0 * m_terrainSize + x0].position.y;
        float y1 = m_vertices[z0 * m_terrainSize + x1].position.y;
        float y2 = m_vertices[z1 * m_terrainSize + x0].position.y;
        float y3 = m_vertices[z1 * m_terrainSize + x1].position.y;

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
}
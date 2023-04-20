#include "Terrain.hpp"

#include <stb_image.h>

#include <filesystem>
#include <fstream>

#include "AssetLoader/TextureLoader.hpp"
#include "Logger.hpp"
#include "Model/Components.hpp"
#include "Model/gl/GLMaterial.hpp"
#include "Terrain.hpp"

namespace Vakol::Controller {
    Terrain::Terrain(const std::string& path) {
        this->m_model =
            std::make_shared<Model::Assets::Model>(LoadHeightMap(LoadImage(path, this->m_size, this->m_size)));
    }

    Terrain::Terrain(const int size, const int iterations, const float filter, const bool random, const int minHeight,
                     const int maxHeight)
        : m_size(size),
          m_model(std::make_shared<Model::Assets::Model>(
              LoadFaultFormation(size, iterations, filter, random, minHeight, maxHeight))){};

    const Model::Assets::Mesh Terrain::LoadHeightMap(unsigned char* data) {
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

    const Model::Assets::Mesh Terrain::LoadFaultFormation(const int size, const int iterations, const float filter,
                                                          const bool random, const int minHeight, const int maxHeight) {
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

    void Terrain::NormalizeValues(std::vector<float>& arr, const int size) {
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

    unsigned char* Terrain::LoadBinaryFile(const std::string& path, size_t& fileSize) {
        unsigned char* result = nullptr;

        std::ifstream in(path, std::ios::in | std::ios::binary);

        if (in) {
            in.seekg(0, std::ios::end);

            fileSize = std::filesystem::file_size(path);
            result = new unsigned char[fileSize];

            if (fileSize > 0) {
                in.seekg(0, std::ios::beg);
                in.read(reinterpret_cast<char*>(result), fileSize);
            } else
                VK_ERROR("Could not read file '{0}'", path);
        } else
            VK_ERROR("Could not open file '{0}'", path);

        return result;
    }

    void Terrain::GenerateDrawable() {
        InitVertices();
        InitIndices();

        Assets::MaterialSpec mat_spec;
        mat_spec.textures.push_back(m_texture);
        Assets::Mesh mesh(m_vertices, m_indices, mat_spec);
        mesh.material()->SetShader("coreAssets/shaders/basic.prog");
        mesh.SetIsStripped(true);
        mesh.strips = m_terrainSize * 2;

        m_texture.Bind();
        mesh.material()->Bind();

        m_entity.AddComponent<Components::Drawable>();
        auto& drawable = m_entity.GetComponent<Components::Drawable>();

        std::vector<Assets::Mesh> meshes;
        meshes.push_back(mesh);
        drawable.model_ptr = std::make_shared<Model::Assets::Model>(meshes);
    }

    void Terrain::LoadHeightMap(const std::string& heightMap) {
        size_t fileSize = 0;
        unsigned char* data = LoadBinaryFile(heightMap, fileSize);

        if (data) {
            m_terrainSize = static_cast<int>(sqrt(fileSize));

            VK_TRACE("Terrain size: {0}", m_terrainSize);

            if ((m_terrainSize * m_terrainSize) != fileSize) {
                VK_ERROR("Terrain size is not a square");
                return;
            }

            // initialise a 2d vector
            m_heightMap = std::vector<std::vector<float>>(m_terrainSize, std::vector<float>(m_terrainSize));

            // read the heightmap data into the 2d vector
            for (int z = 0; z < m_terrainSize; z++) {
                for (int x = 0; x < m_terrainSize; x++) {
                    m_heightMap[z][x] = static_cast<float>(data[z * m_terrainSize + x]) * 0.1f;

                    if(m_heightMap[z][x] < m_minHeight)
                    {
                        m_minHeight = m_heightMap[z][x];
                    }
                    else if(m_heightMap[z][x] > m_maxHeight)
                    {
                        m_maxHeight = m_heightMap[z][x];
                    }
                }
            }

            delete[] data;
        }
    }

    void Terrain::LoadTexture(const std::string& textureFile) {
        if (!std::filesystem::exists(textureFile)) {
            VK_ERROR("Texture file does not exist");
            return;
        }

        m_texture = Assets::GLTexture(textureFile);
    }

    void Terrain::InitVertices() {
        m_vertices.resize(m_terrainSize * m_terrainSize);

        // generate a grid of vertices
        for (int z = 0; z < m_terrainSize; z++) {
            for (int x = 0; x < m_terrainSize; x++) {
                m_vertices[z * m_terrainSize + x].position = glm::vec3(x, m_heightMap[z][x] * m_yScale - m_yShift, z);

                m_vertices[z * m_terrainSize + x].uv =
                    glm::vec2(x / static_cast<float>(m_terrainSize), z / static_cast<float>(m_terrainSize));
            }
        }
    }

    void Terrain::InitIndices() {
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

    float Terrain::getHeight(float x, float z) {
        // get the height of the terrain at a given x and z coordinate
        // this is done by interpolating the height of the 4 vertices that surround the point
        // the height is then interpolated between the 4 vertices

        if (x < 0 || x >= m_size || z < 0 || z >= m_size) {
            return 0.0f;
        }

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

    const std::vector<Vertex>& Terrain::GetVertices() { return m_vertices; }

    const std::vector<unsigned int>& Terrain::GetIndices() { return m_indices; }

    std::vector<unsigned int>& Terrain::ConvertStripToTriangles() {
        for (size_t i = 2; i < m_indices.size(); ++i) {
            // Reverse the winding order for even triangles
            if (i % 2 == 0) {
                m_triangleIndices.push_back(m_indices[i - 2]);
                m_triangleIndices.push_back(m_indices[i - 1]);
                m_triangleIndices.push_back(m_indices[i]);
            } else {
                m_triangleIndices.push_back(m_indices[i - 1]);
                m_triangleIndices.push_back(m_indices[i - 2]);
                m_triangleIndices.push_back(m_indices[i]);
            }
        }

        return m_triangleIndices;
    }

    std::vector<float> Terrain::GetHeightMap1D() const
    {

        std::vector<float> ret;
        for (const auto& vec : m_heightMap)
        {
            for (const float& value : vec)
            {
                ret.push_back(value);
            }
        }

        return ret;
    }

    int Terrain::GetSize() const
    {
        return m_terrainSize;
    }

    float Terrain::GetScale() const
    {
        return m_yScale;
    }

    float Terrain::GetShift() const
    {
        return m_yShift;
    }

    float Terrain::GetMinHeight() const
    {
        return m_minHeight;
    }

    float Terrain::GetMaxHeight() const
    {
        return m_maxHeight;
    }

}  // namespace Vakol::Controller

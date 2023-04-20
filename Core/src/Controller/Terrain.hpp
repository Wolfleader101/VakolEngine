#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

#include "Model/Entity.hpp"
#include "Model/VertexArray.hpp"
#include "Model/gl/GLTexture.hpp"

using namespace Vakol::Model;

namespace Vakol::Controller {

    class Terrain {
       public:
        Terrain();

        void LoadHeightMap(const std::string& heightMap);

        void LoadTexture(const std::string& textureFile);

        void LoadFaultFormation(unsigned int terrainSize);

        void GenerateDrawable();

        float getHeight(float x, float z);

        Entity& GetEntity() { return m_entity; }
        void SetEntity(Entity& entity) { m_entity = entity; }

        const std::vector<Vertex>& GetVertices();
        const std::vector<unsigned int>& GetIndices();

        std::vector<unsigned int>& ConvertStripToTriangles();
        std::vector<float> GetHeightMap1D() const;

        int GetSize() const;

        float GetScale() const;
        float GetShift() const;

        float GetMinHeight() const;
        float GetMaxHeight() const;


       private:
        void InitVertices();
        void InitIndices();
        void InitGL();
        unsigned char* LoadBinaryFile(const std::string& fileName, size_t& fileSize);

        int m_terrainSize;
        std::vector<std::vector<float>> m_heightMap;
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
        std::vector<unsigned int> m_triangleIndices;
        Assets::GLTexture m_texture;

        Entity m_entity;

        float m_yScale = 0.75f;
        float m_yShift = 8.0f;

        float m_minHeight = -25.0f;
        float m_maxHeight = 100.0f;
        float m_firFilter = 0.75f;
        int m_iterations = 50;
    };
}  // namespace Vakol::Controller
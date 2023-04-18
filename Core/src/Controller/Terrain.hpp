#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

#include "Model/Entity.hpp"
#include "Model/VertexArray.hpp"
#include "Model/gl/GLTexture.hpp"

using namespace Vakol::Model;

namespace Vakol::Controller 
{

    class Terrain 
    {
    public:
        Terrain(Entity& entity);

        void LoadHeightMap(const std::string& heightMap);

        void LoadFaultFormation(unsigned int terrainSize);

        void Generate();

        float GetHeight(float x, float z);

        const int GetTerrainSize() const { return m_terrainSize; }

        Entity& GetEntity() { return m_entity; }

    private:
        void GenerateDrawable();

        void InitVertices();
        void InitIndices();

        int m_terrainSize;
        
        std::vector<std::vector<float>> m_heightMap;

        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;

        Entity& m_entity;

        float m_yScale = 0.75f;
        float m_yShift = 8.0f;

        float m_minHeight = -25.0f;
        float m_maxHeight = 100.0f;
        float m_firFilter = 0.75f;

        int m_iterations = 128;
    };
}
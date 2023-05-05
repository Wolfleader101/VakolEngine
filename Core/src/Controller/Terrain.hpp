#pragma once

#include <Model/Assets/Model.hpp>
#include <memory>
#include <string>
#include <vector>

namespace Vakol::Controller {
    class Terrain {
       public:
        // Terrain(const std::string& path);

        Terrain(const int size, float scale, const int octaves, const float persistence, const float lacunarity);
        Terrain(const int size, const int iterations, const float filter, const bool random, const int minHeight,
                const int maxHeight);

        Terrain(const std::string& path);

        std::shared_ptr<Model::Assets::Model> GetModel() const { return this->m_model; }

        const int GetSize() const { return this->m_size; }

        const float GetHeight(const float x, const float z) const;

        const std::vector<float>& GetHeightMap() const;

        void SetHeightMap(std::vector<float>& heightMap) { m_heightMap = std::move(heightMap); }

        ~Terrain(){};

       private:
        void GenerateStaticVertices(const int size);
        const Model::Assets::Mesh LoadHeightMap(unsigned char* data);
        const Model::Assets::Mesh LoadFaultFormation(const int size, const int iterations, const float filter,
                                                     const bool random, const int minHeight, const int maxHeight);

        const Model::Assets::Mesh LoadCLODTerrain(const int size);

        struct Point {
            int x = 0;
            int z = 0;

            const bool operator==(const Point& other) const { return this->x == other.x && this->z == other.z; }

            const Point operator-(const Point& other) const { return {this->x - other.x, this->z - other.z}; }
        };

        std::vector<float> ConvertValues(unsigned char* data);

        void GenRandomPoints(Point& p1, Point& p2, const int size);
        void NormalizeValues(std::vector<float>& arr, const int size);

        void ApplyFIRFilter(std::vector<float>& arr, const int size, const float filter);
        float FIRSinglePass(std::vector<float>& arr, const int index, const float prev, const float filter);

        std::vector<float> m_heightMap;

        std::shared_ptr<Model::Assets::Model> m_model;

        int m_size;

        int m_minHeight;
        int m_maxHeight;
    };
}  // namespace Vakol::Controller
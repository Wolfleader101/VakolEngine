#pragma once

#include <Model/Assets/Model.hpp>
#include <memory>
#include <string>
#include <vector>

namespace Vakol::Controller {
    class Terrain {
       public:
        Terrain(const std::string& path);

        Terrain(const int size, const int iterations, const float filter, const bool random, const int minHeight,
                const int maxHeight);

        std::shared_ptr<Model::Assets::Model> GetModel() const {
            return std::make_shared<Model::Assets::Model>(this->m_model);
        }

        const int GetSize() const { return this->m_size; }

        float GetHeight(const float x, const float z) const;

        ~Terrain(){};

       private:
        const Model::Assets::Mesh LoadHeightMap(unsigned char* data);
        const Model::Assets::Mesh LoadFaultFormation(const int size, const int iterations, const float filter,
                                                     const bool random, const int minHeight, const int maxHeight);

        struct Point {
            int x = 0;
            int z = 0;

            const bool operator==(const Point& other) const { return this->x == other.x && this->z == other.z; }

            const Point operator-(const Point& other) const { return {this->x - other.x, this->z - other.z}; }
        };

        void GenRandomPoints(Point& p1, Point& p2, const int size);
        void NormalizeValues(std::vector<float>& arr, const int size);

        void ApplyFIRFilter(std::vector<float>& arr, const int size, const float filter);
        float FIRSinglePass(std::vector<float>& arr, const int index, const float prev, const float filter);

        std::vector<float> m_heightMap;

        Model::Assets::Model m_model;

        int m_size = 0;

        int m_minHeight = 0;
        int m_maxHeight = 0;
    };
}  // namespace Vakol::Controller
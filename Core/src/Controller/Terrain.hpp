#pragma once

#include <Model/Assets/Model.hpp>
#include <memory>
#include <string>
#include <vector>

namespace Vakol::Controller {
    class Terrain {
       public:
        Terrain() = default;

        void SetModel(Model::Assets::Mesh&& mesh) { this->m_model = std::make_shared<Model::Assets::Model>(mesh); }
        void SetModel(Model::Assets::Model&& model) {
            this->m_model = std::make_shared<Model::Assets::Model>(std::move(model));
        }
        std::shared_ptr<Model::Assets::Model> GetModel() const { return this->m_model; }

        void SetData(const unsigned char* data);

        int GetSize() const { return this->m_size; }

        void SetSize(const int size) { this->m_size = size; }

        float GetHeight(float x, float z) const;

        Model::Assets::Mesh load_height_map_mesh() const;
        Model::Assets::Mesh load_clod_terrain_mesh() const;

        const std::vector<float>& GetHeightMap() const { return this->m_height_map; }
        const std::vector<unsigned char>& GetData() const { return this->m_data; };

        void SetMinMax(std::pair<float, float>&& minmax) {
            this->m_min_height = static_cast<int>(minmax.first);
            this->m_max_height = static_cast<int>(minmax.second);
        }
        [[nodiscard]] std::pair<float, float> GetMinMax() const { return {this->m_min_height, this->m_max_height}; }

       private:
        Model::Assets::Mesh load_fault_formation_mesh(const int size, const int iterations, const float filter,
                                                      const bool random, const int minHeight, const int maxHeight);

        struct Point {
            int x = 0;
            int z = 0;

            bool operator==(const Point& other) const { return this->x == other.x && this->z == other.z; }

            Point operator-(const Point& other) const { return {this->x - other.x, this->z - other.z}; }
        };

        static void GenRandomPoints(Point& p1, Point& p2, const int size);
        void NormalizeValues(std::vector<float>& arr, const int size);

        void SetHeightMap();

        static void ApplyFIRFilter(std::vector<float>& arr, const int size, const float filter);
        static float FirSinglePass(std::vector<float>& arr, const int index, const float prev, const float filter);

        std::shared_ptr<Model::Assets::Model> m_model;

        int m_size = 0;

        int m_min_height = 0;
        int m_max_height = 0;

        std::vector<float> m_height_map;
        std::vector<unsigned char> m_data;
    };

    Terrain LoadHeightMapTerrain(std::string&& path, float min, float max);
    Terrain LoadCLODTerrain(std::string&& path);
}  // namespace Vakol::Controller
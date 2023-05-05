#pragma once

#include <Model/Assets/Model.hpp>
#include <memory>
#include <string>
#include <vector>

namespace Vakol::Controller
{
    class Terrain
	{
    public:
        Terrain() = default;

        void SetModel(Model::Assets::Mesh&& mesh) { this->m_model = std::make_shared<Model::Assets::Model>(mesh); }
        void SetModel(Model::Assets::Model&& model) { this->m_model = std::make_shared<Model::Assets::Model>(std::move(model)); }
        [[nodiscard]] std::shared_ptr<Model::Assets::Model> GetModel() const { return this->m_model; }

        void SetData(const unsigned char* data);

        [[nodiscard]] int GetSize() const { return this->m_size; }
        
        void SetSize(const int size) { this->m_size = size; }

        [[nodiscard]] float GetHeight(const float x, const float z) const;
        
        [[nodiscard]] Model::Assets::Mesh load_height_map_mesh() const;
    	Model::Assets::Mesh load_clod_terrain_mesh();
    private:

        Model::Assets::Mesh load_fault_formation_mesh(const int size, const int iterations, const float filter, const bool random, const int minHeight, const int maxHeight);


        struct Point
    	{
            int x = 0;
            int z = 0;

            bool operator==(const Point& other) const { return this->x == other.x && this->z == other.z; }

            Point operator-(const Point& other) const { return {this->x - other.x, this->z - other.z}; }
        };

        static void GenRandomPoints(Point& p1, Point& p2, const int size);
        static void NormalizeValues(std::vector<float>& arr, const int size);

        static void ApplyFIRFilter(std::vector<float>& arr, const int size, const float filter);
        static float FirSinglePass(std::vector<float>& arr, const int index, const float prev, const float filter);

        std::shared_ptr<Model::Assets::Model> m_model;

        int m_size = 0;

        int m_min_height = 0;
        int m_max_height = 0;

        std::vector<float> m_height_map;
        std::vector<unsigned char> m_data;
    };

    Terrain LoadHeightMapTerrain(std::string&& path);
    Terrain LoadCLODTerrain(std::string&& path);
}
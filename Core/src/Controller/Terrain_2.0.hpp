#pragma once

#include <string>
#include <vector>
#include <memory>

#include <Model/Assets/Model.hpp>

namespace Vakol::Controller
{
    class Terrain_2
    {
    public:
        Terrain_2(const std::string& path) : m_terrain(LoadHeightMap(path)) {};

        std::shared_ptr<Model::Assets::Model> get() const { return std::make_shared<Model::Assets::Model>(m_terrain); }

        ~Terrain_2() {};
        
    private:
        Model::Assets::Mesh LoadHeightMap(const std::string& path) const;
        Model::Assets::Mesh LoadFaultFormation(const int size) const;

        std::vector<float> m_heightMap;

        Model::Assets::Model m_terrain;

        int m_size = 0;
    };
}
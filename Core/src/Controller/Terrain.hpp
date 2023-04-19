#pragma once

#include <string>
#include <vector>
#include <memory>

#include <Model/Assets/Model.hpp>

namespace Vakol::Controller
{
    class Terrain
    {
    public:
        Terrain(const std::string& path);
        
        Terrain(const int size, const int iterations, const float filter, const bool random = true) : 
            m_size(size), m_terrain(LoadFaultFormation(size, iterations, filter, random)) {};

        std::shared_ptr<Model::Assets::Model> get() const { return std::make_shared<Model::Assets::Model>(this->m_terrain); }

        const int get_size() const { return this->m_size; }

        ~Terrain() {};
        
    private:
        const Model::Assets::Mesh LoadHeightMap(unsigned char* data);
        const Model::Assets::Mesh LoadFaultFormation(const int size, const int iterations, const float filter, const bool random);

        std::vector<float> m_heightMap;

        Model::Assets::Model m_terrain;

        int m_size = 0;

        int m_minHeight = 0;
        int m_maxHeight = 0;
    };
}
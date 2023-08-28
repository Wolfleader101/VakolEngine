#pragma once

#include <crossguid/guid.hpp>

#include "ECS/Components.hpp"
#include "Math/Math.hpp"
#include "Rendering/Assets/Mesh.hpp"
#include "Rendering/RenderAPI.hpp"
#include "Rendering/RenderData.hpp"

namespace Vakol
{
    /**
     * @brief A class for generating 3D cubes
     *
     */
    class Cube
    {
      public:
        /**
         * @brief Construct a new Cube object
         *
         */
        Cube();
        /**
         * @brief Construct a new Cube object
         * @param inputName The mesh name of the cube
         * @param inputID The mesh ID of the cube
         *
         */
        Cube(std::string inputName, std::string inputID);
        /**
         * @brief A function which returns the mesh name of the cube
         *
         */
        std::string GetName();
        /**
         * @brief A function which returns the mesh of the cube object
         *
         */
        Rendering::Assets::Mesh& GetCubeMesh();
        /**
         * @brief A function which returns the indices of the current cube
         *
         */
        std::vector<unsigned int>& GetIndices();
        /**
         * @brief A function which returns the vertices of the current cube
         *
         */
        std::vector<Rendering::Vertex>& GetVertices();
        /**
         * @brief Destroy the cube object
         *
         */
        ~Cube();

      private:
        /**
         * @brief Generate the data for a 3D cube
         *
         */
        void GenerateData();

        /**
         * @brief The mesh object of the cube
         *
         */
        Rendering::Assets::Mesh mesh;
        /**
         * @brief The original vertices of the cube before rotations, scaling, etc.
         *
         */
        std::vector<Rendering::Vertex> originalVertices;
    };
} // namespace Vakol

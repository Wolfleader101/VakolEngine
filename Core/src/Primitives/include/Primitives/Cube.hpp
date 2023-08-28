#pragma once

#include "ECS/Components.hpp"
#include "Math/Math.hpp"
#include "Rendering/Assets/Mesh.hpp"
#include "Rendering/RenderAPI.hpp"
#include "Rendering/RenderData.hpp"
#include "Utils/GUID.hpp"

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
         * @brief Construct a new default Cube object
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
         * @brief Construct a new Cube object with half extents
         * @param inputName The mesh name of the cube
         * @param inputID The mesh ID of the cube
         * @param halfExtents The half extents of the cube
         *
         */
        Cube(std::string inputName, std::string inputID, Math::Vec3 halfExtents);
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
         * @brief Generate the data for a 3D cube based on half extents
         * @param halfExtents The half extents of the cube (Overloaded with a zeroed out vector)
         *
         */
        void GenerateData(Math::Vec3 halfExtents = Math::Vec3(0.0, 0.0, 0.0));

        /**
         * @brief The mesh object of the cube
         *
         */
        Rendering::Assets::Mesh mesh;
    };
} // namespace Vakol

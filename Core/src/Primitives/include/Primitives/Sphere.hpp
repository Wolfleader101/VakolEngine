#pragma once

#include "Math/Math.hpp"
#include "Rendering/Assets/Mesh.hpp"
#include "Rendering/RenderData.hpp"
#include "ECS/Components.hpp"

namespace Vakol
{
    /**
     * @brief A class for generating 3D spheres
     *
     */
    class Sphere
    {
        public:
            /**
            * @brief Construct a new Sphere object
            *
            */
            Sphere();
            /**
            * @brief Construct a new Sphere object with a position, radius, stacks and sectors
            * @param inputTransform The transform of the sphere
            * @param inputRadius The radius of the sphere
            * @param inputStacks The number of stacks (latitude) of the sphere
            * @param inputSectors The number of sectors (longitude) of the sphere
            * @param inputName The name of the sphere
            *
            */
            Sphere(Components::Transform inputTransform, double inputRadius, unsigned inputStacks, unsigned inputSectors, std::string inputName);
            /**
             * @brief A function which returns the name of the current sphere
             *
             */
            std::string GetName();
            /**
             * @brief A function which returns the indices of the current sphere
             *
             */
            std::vector<unsigned int> GetIndices();
            /**
             * @brief A function which returns the vertices of the current sphere
             *
             */
            std::vector<Rendering::Vertex> GetVertices();
            /**
            * @brief Destroy the Sphere object
            *
            */
            ~Sphere();
        private:
            /**
            * @brief Generate the data for a 3D sphere
            * @param inputTransform The transform of the sphere
            * @param inputRadius The radius of the sphere
            * @param inputStacks The number of stacks (latitude) of the sphere
            * @param inputSectors The number of sectors (longitude) of the sphere
            * @param inputName The name of the sphere
            *
            */
            void GenerateData(Components::Transform inputTransform, double inputRadius, unsigned inputStacks, unsigned inputSectors, std::string inputName);

            /**
             * @brief sphereTransform The transform of the sphere
             *
             */
            Components::Transform sphereTransform;

            /**
             * @brief The number of stacks (latitude) of the sphere
             *
             */
            unsigned stacks;
            /**
             * @brief The number of sectors (longitude) of the sphere
             *
             */
            unsigned sectors;

            /**
             * @brief The name of the sphere
             *
             */
            std::string name;

            /**
             * @brief The mesh object of the sphere
             *
             */
            Rendering::Assets::Mesh mesh;
            /**
             * @brief The model object of the sphere
             *
             */
            Rendering::Assets::Model model;
            /**
             * @brief The original vertices of the sphere before rotations
             *
             */
            std::vector<Rendering::Vertex> originalVertices;
    };
} // namespace Vakol
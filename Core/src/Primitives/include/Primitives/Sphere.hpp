#pragma once

#include "Math/Math.hpp"
#include "Rendering/Assets/Mesh.hpp"
#include "Rendering/RenderData.hpp"

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
            * @brief Construct a new Sphere object with a position, radius, stacks and sectors
            * @param inputPosition The position of the sphere
            * @param inputRadius The radius of the sphere
            * @param inputStacks The number of stacks (latitude) of the sphere
            * @param inputSectors The number of sectors (longitude) of the sphere
            * @param inputName The name of the sphere
            *
            */
            Sphere(Math::Vec3& inputPosition, double inputRadius, unsigned inputStacks, unsigned inputSectors, std::string inputName);
            /**
             * @brief A function which returns the name of the current sphere
             *
             */
            std::string GetName();
            /**
             * @brief A function which scales the current sphere
             * @param inputScale The new scale of the sphere
             *
             */
            void SetScale(Math::Vec3& inputScale);
            /**
             * @brief A function which moves the sphere to a new position
             * @param inputPosition The new position of the sphere
             *
             */
            void SetPosition(Math::Vec3& inputPosition);
            /**
             * @brief A function which rotates the sphere to a new position
             * @param inputRotation The new rotation of the sphere
             *
             */
            void SetRotation(Math::Quat& inputRotation);
            /**
            * @brief Destroy the Sphere object
            *
            */
            ~Sphere();
        private:
            /**
            * @brief Generate the data for a 3D sphere
            * @param inputPosition The position of the sphere
            * @param inputRadius The radius of the sphere
            * @param inputStacks The number of stacks (latitude) of the sphere
            * @param inputSectors The number of sectors (longitude) of the sphere
            * @param inputName The name of the sphere
            *
            */
            void GenerateData(Math::Vec3& inputPosition, double inputRadius, unsigned inputStacks, unsigned inputSectors, std::string inputName);

            /**
            * @brief The position of the sphere
            *
            */
            Math::Vec3 position;
            /**
            * @brief The scale of the sphere
            *
            */
            Math::Vec3 scale;
            /**
             * @brief The rotation of the sphere
             *
             */
            Math::Quat rotation;

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
             * @brief The original vertices of the sphere before rotations
             *
             */
            std::vector<Rendering::Vertex> originalVertices;
    };
} // namespace Vakol
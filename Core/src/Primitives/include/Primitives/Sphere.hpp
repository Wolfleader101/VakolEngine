#pragma once

#include "Math/Math.hpp"
#include "Rendering/Assets/Mesh.hpp"

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
            * @brief Construct a new Sphere object with default values
            *
            */
            Sphere();
            /**
            * @brief Construct a new Sphere object with a position, radius, stacks and sectors
            * @param inputPosition The position of the sphere
            * @param inputRadius The radius of the sphere
            * @param inputStacks The number of stacks (latitude) of the sphere
            * @param inputSectors The number of sectors (longitude) of the sphere
            * @param inputName The name of the sphere
            *
            */
            Sphere(Math::Vec3 inputPosition, double inputRadius, unsigned inputStacks, unsigned inputSectors, std::string inputName);
            /**
            * @brief Generate the data for a 3D sphere
            * @param inputPosition The position of the sphere
            * @param inputRadius The radius of the sphere
            * @param inputStacks The number of stacks (latitude) of the sphere
            * @param inputSectors The number of sectors (longitude) of the sphere
            * @param inputName The name of the sphere
            *
            */
            void GenerateData(Math::Vec3 inputPosition, double inputRadius, unsigned inputStacks, unsigned inputSectors, std::string inputName);
            /**
             * @brief A function which returns the name of the current sphere
             *
             */
            std::string GetName();
            /**
             * @brief A function which scales the current sphere
             *
             */
            void SetScale(Math::Vec3 inputScale);
            /**
            * @brief Destroy the Sphere object
            *
            */
            ~Sphere();
        private:
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
    };
} // namespace Vakol
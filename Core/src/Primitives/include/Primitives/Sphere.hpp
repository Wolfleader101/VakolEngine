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
            * @brief Construct a new Sphere object
            *
            */
            Sphere();
            /**
            * @brief Create a 3D Sphere object
            * @param position The position of the sphere
            * @param radius The radius of the sphere
            * @param stacks The number of stacks (latitude) of the sphere
            * @param sectors The number of sectors (longitude) of the sphere
            *
            */
            void CreateSphere(Math::Vec3 position, double radius, unsigned stacks, unsigned sectors);
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
             * @brief The mesh object of the sphere
             *
             */
            Rendering::Assets::Mesh mesh;
    };
} // namespace Vakol
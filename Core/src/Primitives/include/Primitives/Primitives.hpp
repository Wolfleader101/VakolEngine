#pragma once

#include "Math/Math.hpp"
#include "Rendering/Assets/Mesh.hpp"

namespace Vakol
{
    /**
     * @brief A class for generating primitive shapes
     *
     */
    class Primitives
    {
        /**
         * @brief A struct for a 3D cube
         *
         */
        struct Cube
        {
            Math::Vec3 position;            ///< The position of the cube
			Math::Vec3 scale;               ///< The scale of the cube
			Math::Quat rotation;            ///< The rotation of the cube

			Rendering::Assets::Mesh mesh;   ///< The mesh object of the cube
		};

        struct Sphere
        {
			Math::Vec3 position;            ///< The position of the sphere
            Math::Vec3 scale;               ///< The scale of the sphere
            Math::Quat rotation;            ///< The rotation of the sphere

            unsigned stacks;                ///< The number of stacks (latitude) of the sphere
            unsigned sectors;               ///< The number of sectors (longitude) of the sphere

            Rendering::Assets::Mesh mesh;   ///< The mesh object of the sphere
        };

        public:
            /**
            * @brief Construct a new Primitives object
            *
            */
            Primitives();
            /**
            * @brief Create a 3D Sphere object
            * @param position The position of the sphere
            * @param scale The scale of the sphere
            * @param rotation The rotation of the sphere
            * @param stacks The number of stacks (latitude) of the sphere
            * @param sectors The number of sectors (longitude) of the sphere
            *
            */
            void CreateSphere(Math::Vec3 position, Math::Vec3 scale, Math::Quat rotation, unsigned stacks, unsigned sectors);
            /**
            * @brief Create a 3D Cube object
            * @param position The position of the cube
            * @param scale The scale of the cube
            * @param rotation The rotation of the cube
            *
            */
            void CreateCube(Math::Vec3 position, Math::Vec3 scale, Math::Quat rotation);
            /**
            * @brief Destroy the Primitives object
            *
            */
            ~Primitives();
        private:
            /**
            * @brief A vector of cube objects
            *
            */
            std::vector<Cube> m_Cubes;
            /**
            * @brief A vector of sphere objects
            *
            */
			std::vector<Sphere> m_Spheres;
    };
} // namespace Vakol
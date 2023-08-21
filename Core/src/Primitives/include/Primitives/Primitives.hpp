#pragma once

#include "Math/Math.hpp"
#include "Rendering/Assets/Mesh.hpp"

#include "Primitives/Sphere.hpp"

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

        public:
            /**
            * @brief Construct a new Primitives object
            *
            */
            Primitives();
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
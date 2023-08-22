#pragma once

#include <unordered_map>

#include "Math/Math.hpp"
#include "Rendering/Assets/Mesh.hpp"
#include "Logger/Logger.hpp"

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
         * @brief An enum for the different types of shapes
         *
         */
        enum ShapeType
        {
            SPHERE                          ///< Sphere shape
        };

        public:
            /**
            * @brief Construct a new Primitives object
            *
            */
            Primitives();
            /**
            * @brief Create a 3D Sphere object
            * @param inputPosition The position of the sphere
            * @param inputRadius The radius of the sphere
            * @param inputStacks The number of stacks (latitude) of the sphere
            * @param inputSectors The number of sectors (longitude) of the sphere
            * @param inputName The name of the sphere
            *
            */
            void CreateSphere(Math::Vec3& inputPosition, double inputRadius, unsigned inputStacks, unsigned inputSectors, std::string inputName);
            /**
             * @brief Scale a selected primitive shape
             * @param type The type of shape to scale
             * @param inputScale The new scale of the shape
             * @param inputName The name of the shape
             *
             */
            void Scale(ShapeType type, Math::Vec3& inputScale, std::string inputName);
            /**
             * @brief Position a selected primitive shape
             * @param type The type of shape to scale
             * @param inputPosition The new position of the shape
             * @param inputName The name of the shape
             *
             */
            void Position(ShapeType type, Math::Vec3& inputPosition, std::string inputName);
            /**
             * @brief Rotate a selected primitive shape
             * @param type The type of shape to rotate
             * @param inputRotation The new rotation of the shape
             * @param inputName The name of the shape
             *
             */
            void Rotation(ShapeType type, Math::Quat& inputRotation, std::string inputName);
            /**
            * @brief Destroy the Primitives object
            *
            */
            ~Primitives();
        private:
            /**
            * @brief A map of all the spheres
            *
            */
            std::unordered_map<std::string, Sphere> m_Spheres;
    };
} // namespace Vakol
#pragma once

#include <unordered_map>
#include <algorithm>

#include <crossguid/guid.hpp>

#include "Logger/Logger.hpp"
#include "Primitives/Sphere.hpp"
#include "Rendering/RenderAPI.hpp" 

namespace Vakol
{
    /**
     * @brief A class for generating primitive shapes
     *
     */
    class Primitives
    {
        public:
            /**
             * @brief An enum for the different types of shapes
             *
             */
            enum ShapeType
            {
                SPHERE ///< Sphere shape
            };
            /**
            * @brief Construct a new Primitives object
            *
            */
            Primitives();
            /**
            * @brief Create a 3D Sphere object
            * @param inputTransform The transform of the sphere
            * @param inputRadius The radius of the sphere
            * @param inputStacks The number of stacks (latitude) of the sphere
            * @param inputSectors The number of sectors (longitude) of the sphere
            * @param inputName The name of the sphere
            *
            */
            void CreateSphere(Components::Transform& inputTransform, double inputRadius, unsigned inputStacks, unsigned inputSectors, std::string inputName); 
            /**
             * @brief Scale a selected primitive shape
             * @param type The type of shape to scale
             * @param inputScale The new scale of the shape
             * @param inputGUID The GUID of the shape
             *
             */
            void Scale(ShapeType type, Math::Vec3& inputScale, xg::Guid inputGUID);
            /**
             * @brief Position a selected primitive shape
             * @param type The type of shape to scale
             * @param inputPosition The new position of the shape
             * @param inputGUID The GUID of the shape
             *
             */
            void Position(ShapeType type, Math::Vec3& inputPosition, xg::Guid inputGUID);
            /**
             * @brief Rotate a selected primitive shape
             * @param type The type of shape to rotate
             * @param inputRotation The new rotation of the shape
             * @param inputGUID The GUID of the shape
             *
             */
            void Rotation(ShapeType type, Math::Quat& inputRotation, xg::Guid inputGUID);
            /**
             * @brief Render a selected primitive shape
             * @param type The type of shape to render
             * @param inputGUID The GUID of the shape
             *
             */
            void RenderShape(ShapeType type, xg::Guid inputGUID); 
            /**
             * @brief Return the GUID of a shape based on its name
             * @param name The name of the shape corrisonding to the GUID
             *
             */
            xg::Guid GetGuidByName(const std::string& name);
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
            std::unordered_map<xg::Guid, Sphere> m_Spheres;
            /**
            * @brief A map of corripsonding names to guids
            *
            */
            std::unordered_map<std::string, xg::Guid> nameToGuidMap; 
    };
} // namespace Vakol
#pragma once

#include <unordered_map>
#include <algorithm>
#include <cstdlib>

#include <crossguid/guid.hpp>

#include "Rendering/Assets/Material.hpp"  
#include "Logger/Logger.hpp"
#include "Rendering/RenderAPI.hpp" 
#include "AssetLoader/AssetLoader.hpp" 

#include "Primitives/Sphere.hpp"
#include "Primitives/Cube.hpp"

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
            * @brief Create a 3D Cube object
            * @param inputTransform The transform of the cube
            * @param inputName The name of the cube
            *
            */
            void CreateCube(Components::Transform& inputTransform, std::string inputName);
            /**
             * @brief Scale a selected primitive shape
             * @param type The type of shape to scale
             * @param inputScale The new scale of the shape
             * @param inputGUID The GUID of the shape
             *
             */ 
            xg::Guid GetGuidByName(const std::string& name);
            /**
             * @brief Return the model of a selected primitive shape based on its GUID
             * @param type The type of shape the model belongs to
             * @param inputGUID The GUID of the shape
             *
             */
            Rendering::Assets::Model& GetModel(ShapeType type, xg::Guid inputGUID); 
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
            std::unordered_map<xg::Guid, Rendering::Assets::Model> m_Spheres;
            /**
            * @brief A map of all the cubes
            *
            */
            std::unordered_map<xg::Guid, Rendering::Assets::Model> m_Cubes;
            /**
            * @brief A map of corripsonding names to guids
            *
            */
            std::unordered_map<std::string, xg::Guid> nameToGuidMap; 
    };
} // namespace Vakol
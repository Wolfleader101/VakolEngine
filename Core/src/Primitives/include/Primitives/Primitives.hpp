#pragma once

#include <algorithm>
#include <cstdlib>
#include <unordered_map>

#include <crossguid/guid.hpp>

#include "AssetLoader/AssetLoader.hpp"
#include "Logger/Logger.hpp"
#include "Rendering/Assets/Material.hpp"
#include "Rendering/RenderAPI.hpp"

#include "Primitives/Cube.hpp"
#include "Primitives/Sphere.hpp"

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
            SPHERE, ///< Sphere shape
            CUBE    ///< Cube shape
        };
        /**
         * @brief Construct a new Primitives object
         *
         */
        Primitives();
        /**
         * @brief Create a 3D Sphere object
         * @param inputRadius The radius of the sphere
         * @param inputStacks The number of stacks (latitude) of the sphere
         * @param inputSectors The number of sectors (longitude) of the sphere
         * @param inputName The name of the sphere
         *
         */
        void CreateSphere(double inputRadius, unsigned inputStacks, unsigned inputSectors, std::string inputName);
        /**
         * @brief Create a 3D Cube object
         * @param inputName The name of the cube
         *
         */
        void CreateCube(std::string inputName);
        /**
         * @brief Scale a selected primitive shape
         * @param type The type of shape to scale
         * @param inputScale The new scale of the shape
         * @param inputGUID The GUID of the shape
         *
         */
        xg::Guid GetGuidByName(const std::string& name);
        /**
         * @brief Return a boolean if the shape exists and return the model through the outModel parameter
         * @param type The type of shape the model belongs to
         * @param inputGUID The GUID of the shape
         * @param outModel The model to return
         *
         */
        bool GetModel(ShapeType type, xg::Guid inputGUID, Rendering::Assets::Model& outModel);
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

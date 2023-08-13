#pragma once

#include <reactphysics3d/reactphysics3d.h>

#include "Math/Math.hpp"
#include <memory>
#include <vector>

#include "Renderer.hpp"
#include "Shader.hpp"

namespace Vakol::Physics
{
    /**
     * @brief Forward declaration of the ScenePhysics class
     */
    class ScenePhysics;
} // namespace Vakol::Physics

namespace Vakol
{
    using Shader = Shader;

    /**
     * @brief DebugRenderer class used for rendering debugging information
     */
    class DebugRenderer
    {
      public:
        /**
         * @brief Set the Shader
         * @param shader the shader to be used
         */
        void SetShader(const std::shared_ptr<Shader>& shader);

        /**
         * @brief Enable or disable the DebugRenderer
         * @param enable whether or not the DebugRenderer is enabled
         */
        void Enable(bool enable);

        /**
         * @brief Check if DebugRenderer is enabled
         * @return bool indicating if DebugRenderer is enabled
         */
        bool IsEnabled() const;

        /**
         * @brief Perform an update on the DebugRenderer
         */
        void Update();

        /**
         * @brief Draw the DebugRenderer
         */
        void Draw() const;

      private:
        /**
         * @brief Default constructor for DebugRenderer
         */
        DebugRenderer() = default;

        /**
         * @brief Constructor for DebugRenderer taking a PhysicsWorld pointer
         * @param WorldPtr the pointer to a PhysicsWorld object
         */
        explicit DebugRenderer(rp3d::PhysicsWorld* WorldPtr);

        /**
         * @brief A struct that holds information about a debug vertex in the physics world
         */
        struct PhysicsDebugVertex
        {
            Math::Vec3 xyz;   ///< position of the debug vertex
            Math::Vec3 color; ///< colour of the debug vertex
        };

        /**
         * @brief Enable world debugging
         */
        void EnableWorldDebug();

        /**
         * @brief Disable world debugging
         */
        void DisableWorldDebug();

        /**
         * @brief Get debug colour
         * @param color the color to be translated
         * @param outColor the resulting Math::Vec3 colour
         */
        void GetDebugColor(const uint32_t& color, Math::Vec3& outColor) const;

        /**
         * @brief Get Triangles for debug rendering
         */
        void GetTriangles();

        /**
         * @brief Get Lines for debug rendering
         */
        void GetLines();

        std::vector<PhysicsDebugVertex> m_DebugData; ///< Debug data in the form of vertices

        rp3d::PhysicsWorld* m_World = nullptr;         ///< Pointer to the PhysicsWorld object
        std::shared_ptr<Shader> m_Shader = nullptr;    ///< Pointer to the Shader object
        rp3d::DebugRenderer* m_rp3dRenderer = nullptr; ///< Pointer to the rp3d::DebugRenderer object

        uint32_t m_VAO{}; ///< Vertex Array Object ID
        uint32_t m_VBO{}; ///< Vertex Buffer Object ID

        /**
         * @brief Declare ScenePhysics as a friend of DebugRenderer, allowing it to access private members
         */
        friend class Physics::ScenePhysics;
    };
} // namespace Vakol
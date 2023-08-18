#pragma once

#include "Components.hpp"
#include "EntityList.hpp"
#include "Physics/ScenePhysics.hpp"

#include "SceneManager/Scene.hpp"

namespace Vakol
{

    class Scene;

    /**
     * @class System
     *
     * @brief Class representing a system in the application.
     */
    class System
    {
      public:
        System() = delete;

        /**
         * @brief Bind a scene to the system.
         *
         * @param scene The scene to bind.
         */
        static void BindScene(Scene& scene);

        // -- READ --
        // if you add a function use the convention Type_Action()
        // will make it more intuitive to find names of funcs

        /**
         * @brief Prepare the scripting system for deserialization.
         *
         * @param lua The shared pointer to the Lua state.
         * @param list The entity list.
         * @param scene The pointer to the scene.
         */
        // static void Script_Deserialize(std::shared_ptr<LuaState> lua, EntityList& list, Scene* scene);

        /**
         * @brief Initialize the physics system for an entity.
         *
         * @param ent The entity to initialize.
         */
        static void Physics_InitEntity(const Entity& ent);

        /**
         * @brief Initialize the physics system.
         */
        static void Physics_Init();

        /**
         * @brief Update the physics system transforms.
         *
         * @param factor The update factor.
         */
        static void Physics_UpdateTransforms(double factor);

        /**
         * @brief Prepare the physics system for serialization.
         */
        static void Physics_SerializationPrep();

      private:
        /**
         * @brief The shared pointer to the ScenePhysics object.
         */
        static std::shared_ptr<Physics::ScenePhysics> m_SP;

        /**
         * @brief The pointer to the entt::registry object.
         */
        static entt::registry* m_registry;

        /**
         * @brief The pointer to the EntityList object.
         */
        static EntityList* Entlist;
    };
} // namespace Vakol
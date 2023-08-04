#pragma once

#include <memory>

#include "Camera.hpp"
#include "Controller/Physics/ScenePhysics.hpp"
#include "Controller/Terrain.hpp"
#include "EntityList.hpp"
#include "Model/Entity.hpp"
#include "Scripting/ScriptTypes.hpp"
#include "Time.hpp"
#include "View/Renderer/Renderer.hpp"

namespace Vakol::Controller {

    /**
     * @class Scene
     *
     * @brief Class representing a scene in the application.
     */
    class Scene {
       public:
        /**
         * @brief Construct a new Scene object.
         *
         * @param name The name of the scene.
         * @param script The Lua script associated with the scene.
         * @param SP The shared pointer to the ScenePhysics object.
         * @param active Whether the scene is active or not.
         */
        Scene(const std::string& name, LuaScript& script, const std::shared_ptr<Physics::ScenePhysics>& SP);

        Scene(const Scene& other) = delete;
        Scene(Scene&& other) = default;

        /**
         * @brief Get the name of the scene.
         *
         * @return const std::string& The name of the scene.
         */
        const std::string& getName() const;

        /**
         * @brief Set the name of the scene.
         *
         * @param newName The new name of the scene.
         */
        void setName(const std::string& newName);

        /**
         * @brief Update the scene.
         *
         * @param time The time information for the update.
         * @param renderer The shared pointer to the renderer.
         */
        void Update(const Time& time, const std::shared_ptr<View::Renderer>& renderer);

        /**
         * @brief Create a new entity in the scene.
         *
         * @param tag The tag of the entity.
         * @param sname The name of the script associated with the entity.
         * @return Entity The created entity.
         */
        Model::Entity CreateEntity(const std::string& tag);

        /**
         * @brief Destroy an entity in the scene.
         *
         * @param entity The entity to destroy.
         */
        void DestroyEntity(Model::Entity entity);

        /**
         * @brief Serialize the scene.
         *
         * @param folder The folder to save the serialized data.
         */
        void Serialize(const std::string& folder) const;

        /**
         * @brief Deserialize the scene.
         *
         * @param folder The folder to load the serialized data from.
         */
        void Deserialize(const std::string& folder);

        /**
         * @brief Whether the scene has been initialized or not.
         */
        bool initialized = false;

        /**
         * @brief The shared pointer to the ScenePhysics object.
         */
        std::shared_ptr<Physics::ScenePhysics> scenePhysics;

        /**
         * @brief Get the camera of the scene.
         *
         * @return Camera& The reference to the camera.
         */
        Camera& GetCamera() { return m_cam; }

        /**
         * @brief Get an entity in the scene by its tag.
         *
         * @param tag The tag of the entity.
         * @return std::shared_ptr<Entity> The shared pointer to the entity.
         */
        std::shared_ptr<Model::Entity> GetEntity(const std::string& tag);

        /**
         * @brief Get the Entity List object
         *
         * @return const EntityList&
         */
        const EntityList& GetEntityList() const { return m_entityList; }

        /**
         * @brief Get the Entity List object
         *
         * @return EntityList&
         */
        EntityList& GetEntityList() { return m_entityList; }

        /**
         * @brief Get the Script object
         *
         * @return LuaScript&
         */
        LuaScript& GetScript() { return m_script; }

       private:
        /**
         * @brief The lua script of the scene.
         *
         */
        LuaScript m_script;

        /**
         * @brief The name of the scene.
         */
        std::string m_name;

        /**
         * @brief The camera of the scene.
         */
        Camera m_cam;

        /**
         * @brief The entity list of the scene.
         */
        EntityList m_entityList;
    };
}  // namespace Vakol::Controller
#pragma once

#include <memory>

#include "Camera.hpp"
#include "Controller/Physics/ScenePhysics.hpp"
#include "Controller/Terrain.hpp"
#include "EntityList.hpp"
#include "LuaState.hpp"
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
         * @param scriptName The name of the Lua script associated with the scene.
         * @param lua The shared pointer to the Lua state.
         * @param SP The shared pointer to the ScenePhysics object.
         * @param active Whether the scene is active or not.
         */
        Scene(const std::string& name, const std::string& scriptName, std::shared_ptr<LuaState> lua,
              const std::shared_ptr<Physics::ScenePhysics>& SP, bool active);

        /**
         * @brief Initialize the scene.
         */
        void Init();

        /**
         * @brief The entity list of the scene.
         */
        EntityList entityList;

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
        Entity CreateEntity(const std::string& tag, const std::string& sname);

        /**
         * @brief Destroy an entity in the scene.
         *
         * @param entity The entity to destroy.
         */
        void DestroyEntity(Entity entity);

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
         * @brief Whether the scene is active or not.
         */
        bool active = false;

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
        Camera& GetCamera() { return cam; }

        /**
         * @brief Get an entity in the scene by its tag.
         *
         * @param tag The tag of the entity.
         * @return std::shared_ptr<Entity> The shared pointer to the entity.
         */
        std::shared_ptr<Entity> GetEntity(const std::string& tag);

        /**
         * @brief The scene globals in Lua.
         */
        sol::table sceneGlobals;

       private:
        /**
         * @brief The shared pointer to the LuaState object.
         */
        std::shared_ptr<LuaState> lua;

        /**
         * @brief The name of the Lua script associated with the scene.
         */
        std::string scriptName;

        /**
         * @brief The name of the scene.
         */
        std::string name;

        /**
         * @brief The camera of the scene.
         */
        Camera cam;
    };
}  // namespace Vakol::Controller
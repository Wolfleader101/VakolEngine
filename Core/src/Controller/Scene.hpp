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

    class Scene {
       public:
        /**
         * @brief Construct a new Scene object
         *
         */
        Scene(std::string& name, const std::string& scriptName, LuaState& lua,
              const std::shared_ptr<Physics::ScenePhysics>& SP, bool active = false);
        /**
         * @brief the entity list of scene
         */
        EntityList entityList;

        const std::string& getName() const;
        void setName(const std::string& newName);

        void Update(const Time& time, const std::shared_ptr<View::Renderer>& renderer);

        Entity CreateEntity(const std::string& tag, const std::string& sname);

        void Serialize(const std::string& folder) const;
        void Deserialize(const std::string& folder);

        bool active = false;

        std::shared_ptr<Physics::ScenePhysics> scenePhysics;
        Camera& GetCamera() { return cam; }
        std::shared_ptr<Entity> GetEntity(const std::string& tag);

        sol::table sceneGlobals;

       private:
        LuaState& lua;
        std::string scriptName;
        std::string name;
        Camera cam;
    };
}  // namespace Vakol::Controller
#pragma once

#include "EntityList.hpp"
#include "LuaState.hpp"
#include "Time.hpp"
namespace Vakol::Controller {
    class Scene {
       public:
        /**
         * @brief Construct a new Scene object
         *
         */
        Scene(const std::string& name, const std::string& scriptName, LuaState& lua);
        /**
         * @brief the entity list of engine
         */
        EntityList entityList;  // to be moved to game/scene but for now it all works

        void Update(const Time& time);

        void CreateEntity(const std::string scriptName = "");

       private:
        LuaState& lua;
        std::string scriptName;
        std::string name;
    };
}  // namespace Vakol::Controller
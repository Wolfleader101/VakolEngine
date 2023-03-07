#pragma once

#include "EntityList.hpp"
#include "LuaState.hpp"
namespace Vakol::Controller {
    class Scene {
       public:
        /**
         * @brief Construct a new Scene object
         *
         */
        Scene(LuaState& lua);
        /**
         * @brief the entity list of engine
         */
        EntityList entityList;  // to be moved to game/scene but for now it all works

        void LoadScript(Entity& entity, std::string scriptName);

       private:
        LuaState& lua;
    };
}  // namespace Vakol::Controller
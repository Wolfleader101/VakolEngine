#pragma once

#include "EntityList.hpp"
namespace Vakol::Controller {
    class Scene {
       public:
        /**
         * @brief Construct a new Scene object
         *
         */
        Scene();
        /**
         * @brief the entity list of engine
         */
        EntityList entityList;  // to be moved to game/scene but for now it all works
    };
}  // namespace Vakol::Controller
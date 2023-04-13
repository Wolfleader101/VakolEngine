#pragma once

#include "EntityList.hpp"
#include "LuaState.hpp"
#include "Time.hpp"
#include "View/Renderer/Renderer.hpp"

namespace Vakol::Controller 
{
    using Vakol::View::Renderer;

    class Scene {
       public:
        /**
         * @brief Construct a new Scene object
         *
         */
        Scene(const std::string& name, const std::string& scriptName, LuaState& lua);
        /**
         * @brief the entity list of scene
         */
        EntityList entityList;

        const std::string& getName() const;
        void setName(const std::string& newName);

        void Update(const Time& time, const std::shared_ptr<Renderer> renderer);

        Model::Entity CreateEntity(const std::string scriptName = "");

        void Serialize(const std::string& folder) const;
        void Deserialize(const std::string& folder);

       private:
        LuaState& lua;
        std::string scriptName;
        std::string name;
    };
}  // namespace Vakol::Controller
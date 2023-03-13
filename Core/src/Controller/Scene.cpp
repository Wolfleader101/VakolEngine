#include "Scene.hpp"

#include <filesystem>

#include <Model/Components.hpp>
#include <Model/Entity.hpp>

#include <Controller/Serialization/Serializable.hpp>


#include "LuaAccess.hpp"
#include "System.hpp"


namespace Vakol::Controller {
    Scene::Scene(const std::string& name, const std::string& scriptName, LuaState& lua)
        : name(name), scriptName(scriptName), lua(lua), entityList() {
        lua.RunFile("scripts/" + scriptName);

        sol::function init = lua.GetState()["init"];

        init(*this);
    }

    void Scene::CreateEntity(const std::string scriptName) {
        auto ent = entityList.CreateEntity();
        if (scriptName.length() != 0) ent.AddComponent<Model::Components::Script>(scriptName, lua);
    }

    void Scene::Update(const Controller::Time& time) {
        lua.RunFile("scripts/" + scriptName);

        sol::function update = lua.GetState()["update"];

        update();

        System::ScriptUpdate(entityList.GetRegistry(), lua);
    }


    void Scene::Serialize(const std::string& folder) const
    {
        std::string SceneFolderLoc = folder + "/" + name;
        // makes assets/scenes if it doesn't exist
        if (!std::filesystem::exists(folder)) std::filesystem::create_directory(folder);
        // makes assets/scenes/scene_name if it doesn't exist
        if (!std::filesystem::exists(SceneFolderLoc)) std::filesystem::create_directory(SceneFolderLoc);

        entityList.Serialize(SceneFolderLoc + "EntityList.json");
        
        //json.Serialize camera...
        
    }

    void Scene::Deserialize(const std::string& folder)
    {
            folder;
    }

}  // namespace Vakol::Controller
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

    namespace fs = std::filesystem;

    void Scene::Serialize(const std::string& folder) const
    {
        
        std::string folderPath = "\\" + folder + "\\" + name;
        fs::path currentPath = fs::current_path();


        try
        {
            currentPath += folderPath;
            fs::create_directory(currentPath);  // creates directory for scene if it doesnt exist
        }
        catch (std::exception e)
        {
            //directory already exists
        }
        

        entityList.Serialize(folderPath + "/EntityList.json");
        
        //json.Serialize camera...
        
    }

    void Scene::Deserialize(const std::string& folder)
    {
            folder;
    }

}  // namespace Vakol::Controller
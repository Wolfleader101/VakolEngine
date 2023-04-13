#include "Scene.hpp"

#include <Controller/Serialization/Serializable.hpp>
#include <Model/Components.hpp>
#include <Model/Entity.hpp>
#include <cereal/archives/json.hpp>
#include <filesystem>

#include "LuaAccess.hpp"
#include "System.hpp"

namespace Vakol::Controller {
    Scene::Scene(const std::string& name, const std::string& scriptName, LuaState& lua)
        : name(name), scriptName(scriptName), lua(lua), entityList(), cam(glm::vec3(0.0f, 0.0f, 1.0f)) {
        lua.RunFile("scripts/" + scriptName);
        System::SetEntityList(entityList);

        sol::function init = lua.GetState()["init"];

        init(*this);
    }

    const std::string& Scene::getName() const { return name; }

    void Scene::setName(const std::string& newName) { name = newName; }

    Model::Entity Scene::CreateEntity(const std::string scriptName) {
        auto ent = entityList.CreateEntity();
        if (scriptName.length() != 0) ent.AddComponent<Model::Components::Script>(scriptName, lua, ent);
        return ent;
    }

    void Scene::Update(const Time& time) {
        System::SetEntityList(entityList);

        lua.RunFile("scripts/" + scriptName);

        sol::function update = lua.GetState()["update"];

        update(*this);

        System::Script_Update(lua, entityList, this);

        System::Drawable_Update();

        cam.Update(time.deltaTime);
    }

    namespace fs = std::filesystem;

    void Scene::Serialize(const std::string& folder) const {
        std::string temp = folder;
        std::replace(temp.begin(), temp.end(), '/', '\\');  // replace / with \\ for filesystem

        std::string folderPath = "\\" + temp + "\\" + name;
        fs::path currentPath = fs::current_path();
        try {
            currentPath += folderPath;
            fs::create_directories(currentPath);  // creates directory for scene if it doesnt exist
        } catch (std::exception e) {
            // directory already exists
        }

        std::string FinalFolder = folder + "/" + name;

        entityList.Serialize(FinalFolder + "/EntityList.json");

        // json.Serialize camera...

        //-- Serialize Scene info
        std::ofstream output(FinalFolder + "/Scene.json");
        if (output.good()) {
            cereal::JSONOutputArchive json(output);

            json(cereal::make_nvp("Scene Name", name));
            json(cereal::make_nvp("Script Name", scriptName));
        }
    }

    void Scene::Deserialize(const std::string& folder) {
        entityList.Deserialize(folder + "/EntityList.json");

        std::ifstream input(folder + "/Scene.json");
        if (input.good()) {
            cereal::JSONInputArchive json(input);
            json(name);
            json(scriptName);
        }
    }

}  // namespace Vakol::Controller
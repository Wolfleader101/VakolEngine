#include "Scene.hpp"

#include <Controller/Serialization/Serializable.hpp>
#include <Model/Components.hpp>
#include <Model/Entity.hpp>
#include <cereal/archives/json.hpp>
#include <filesystem>

#include "LuaAccess.hpp"
#include "System.hpp"

#include <Controller/Camera.hpp>

namespace Vakol::Controller {
    Scene::Scene(const std::string& name, const std::string& scriptName, LuaState& lua,
                 std::shared_ptr<Physics::ScenePhysics> SP, bool active)
        : name(name),
          scriptName(scriptName),
          lua(lua),
          entityList(),
          scenePhysics(SP),
          active(active),
          cam(glm::vec3(0.0f, 0.0f, 2.0f)) {
        lua.RunFile("scripts/" + scriptName);
        System::BindScene(*this);

        sol::function init = lua.GetState()["init"];

        init(*this);

        scenePhysics->Init();

        //System::Physics_AddTerrain();

        auto entity = entityList.CreateEntity();

        entity.GetComponent<Components::Transform>().pos = { 1, 10, 1 };

        entity.AddComponent<RigidBody>(scenePhysics, std::nullopt);
        entity.AddComponent<Drawable>("coreAssets/models/cube.obj");

        

        auto& rigid = entity.GetComponent<RigidBody>();

        rigid.Type = (rp3d::BodyType) 2;

        entity.AddComponent<Collider>(rigid, std::nullopt);

        auto& collider = entity.GetComponent<Collider>();
        auto& draw = entity.GetComponent<Drawable>();
        draw.model_ptr->SetShader("coreAssets/shaders/basic.prog");
        auto bounds = Components::getBounds(draw);

        collider.bounds = bounds;

        System::Physics_InitObject(rigid, collider, std::nullopt, entity.GetComponent<Transform>());

        int x = 0;
    }

    const std::string& Scene::getName() const { return name; }

    void Scene::setName(const std::string& newName) { name = newName; }

    Model::Entity Scene::CreateEntity(const std::string scriptName) {
        auto ent = entityList.CreateEntity();
        if (scriptName.length() != 0) ent.AddComponent<Model::Components::Script>(scriptName, lua, ent, *this);
        return ent;
    }

    void Scene::Update(const Time& time, const std::shared_ptr<View::Renderer> renderer) {
        lua.RunFile("scripts/" + scriptName);

        sol::function update = lua.GetState()["update"];

        update(*this);

        scenePhysics->Update(time, cam);

        System::Script_Update(lua, entityList, this);

        System::Drawable_Update(time, cam, renderer);

        cam.Update(time.deltaTime);

    	
    }

    namespace fs = std::filesystem;

    void Scene::Serialize(const std::string& folder) const
	{

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

        System::Physics_SerializationPrep();
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

        System::BindScene(*this);
        System::Drawable_Init();
        System::Physics_Init();

        std::ifstream input(folder + "/Scene.json");
        if (input.good()) {
            cereal::JSONInputArchive json(input);
            json(name);
            json(scriptName);
        }
    }

};  // namespace Vakol::Controller
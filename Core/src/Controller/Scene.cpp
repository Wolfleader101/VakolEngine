#include "Scene.hpp"

#include <Controller/Camera.hpp>
#include <Model/Components.hpp>
#include <Model/Entity.hpp>
#include <cereal/archives/json.hpp>
#include <filesystem>

#include "LuaAccess.hpp"
#include "System.hpp"
#include "SolSerialize.hpp"

namespace Vakol::Controller
{
    Scene::Scene(const std::string& name, const std::string& scriptName, std::shared_ptr<LuaState> lua, const std::shared_ptr<ScenePhysics>& SP, const bool active)
        :
	      active(active),
	      scenePhysics(SP),
		  lua(lua),
          scriptName(scriptName),
          name(name),
          cam(glm::vec3(0.0f, 0.0f, 2.0f)) {}

    void Scene::Init()
	{
        lua->RunFile("scripts/" + scriptName);

        sceneGlobals = lua->GetState().create_named_table(name);

        lua->GetState()["scene"] = this;

        lua->RunFunction("init");

        initialized = true;

        System::BindScene(*this);
    }

    const std::string& Scene::getName() const { return name; }

    void Scene::setName(const std::string& newName) { name = newName; }

    Entity Scene::CreateEntity(const std::string& tag, const std::string& sname)
	{
        auto ent = entityList.CreateEntity();

        ent.GetComponent<Tag>().tag = tag;

        if(!ent.GetComponent<GUID>().id.isValid())
            ent.GetComponent<GUID>().GenNewGUID();

        if (!sname.empty()) ent.AddComponent<Script>(sname, lua, ent, *this);

        return ent;
    }

    void Scene::DestroyEntity(const Entity entity) { entityList.RemoveEntity(entity); }

    void Scene::Update(const Time& time, const std::shared_ptr<View::Renderer>& renderer)
	{
        lua->RunFile("scripts/" + scriptName);

        lua->GetState()["scene"] = this;
        lua->RunFunction("update");

        scenePhysics->Update(time, cam);

        System::Script_Update(lua, entityList, this);

        System::Drawable_Update(time, renderer);

        cam.Update();
    }

    std::shared_ptr<Entity> Scene::GetEntity(const std::string& tag)
	{
        Entity ent;

        entityList.m_Registry.view<Tag>().each([&](auto entity, auto& tagComponent) 
        {
            if (tagComponent.tag == tag)
                ent = entityList.GetEntity(static_cast<unsigned int>(entity));

        });

        return std::make_shared<Entity>(ent);
    }

    namespace fs = std::filesystem;

    void Scene::Serialize(const std::string& folder) const
	{
        std::string temp = folder;
        std::replace(temp.begin(), temp.end(), '/', '\\');  // replace / with \\ for filesystem

        const std::string folderPath = "\\" + temp + "\\" + name;

        fs::path currentPath = fs::current_path();

        try 
        {
            currentPath += folderPath;
            fs::create_directories(currentPath);  // creates directory for scene if it doesnt exist
        } catch (...) 
        {
            // directory already exists
        }

        System::Physics_SerializationPrep();
        const std::string FinalFolder = folder + "/" + name;
        entityList.Serialize(FinalFolder + "/EntityList.json");

        //-- Serialize Scene info
        std::ofstream output(FinalFolder + "/Scene.json");

        if (output.good()) 
        {
            cereal::JSONOutputArchive json(output);

            json(cereal::make_nvp("Scene Name", name));
            json(cereal::make_nvp("Script Name", scriptName));
            json(cereal::make_nvp("camera", cam));
            
        }

        std::ofstream globalOutput(FinalFolder + "/Globals.json");

        if(globalOutput.good())
        {
            cereal::JSONOutputArchive json(globalOutput);

            SolTableData globals;
            ConvertSolToMap(sceneGlobals, globals);
            json(CEREAL_NVP(globals));
        }
    }

    void Scene::Deserialize(const std::string& folder) {

        std::ifstream globalInput(folder + "/Globals.json");

        if(globalInput.good())
        {
            cereal::JSONInputArchive json(globalInput);

            SolTableData globals;
            json(globals);

            ConvertMapToSol(globals, sceneGlobals);

            VK_TRACE("{0}", sceneGlobals["player"]["health"].get<float>());
        }
        
        entityList.Deserialize(folder + "/EntityList.json");

        System::BindScene(*this);
        System::Drawable_Init();
        System::Physics_Init();
        System::Script_Deserialize(lua, entityList, this);

        std::ifstream input(folder + "/Scene.json");

        if (input.good()) 
        {
            cereal::JSONInputArchive json(input);
            json(name);
            json(scriptName);
            json(cam);
        }

        
    }

};  // namespace Vakol::Controller
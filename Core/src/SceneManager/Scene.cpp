#include "SceneManager/Scene.hpp"

#include <algorithm>
#include <cereal/archives/json.hpp>
#include <filesystem>

#include "Camera/Camera.hpp"
#include "Utils/GUID.hpp"
#include "ECS/Components.hpp"
#include "ECS/Entity.hpp"

#include "Serialisation/SolSerialize.hpp"

namespace Vakol
{
    Scene::Scene(const std::string& name, LuaScript& script, PhysicsScene& physicsScene, const bool debugEnabled)
        : m_debugEnabled(debugEnabled), m_script(std::move(script)), m_name(name), m_physicsScene(physicsScene)
    {
    }

    const std::string& Scene::getName() const
    {
        return m_name;
    }

    void Scene::setName(const std::string& newName)
    {
        m_name = newName;
    }

    Entity Scene::CreateEntity(const std::string& tag)
    {
        auto ent = m_entityList.CreateEntity();

        ent.GetComponent<Components::Tag>().tag = tag;

        if (!ent.GetComponent<GUID>().IsValid())
            ent.GetComponent<GUID>().GenNewGUID();

        return ent;
    }

    void Scene::DestroyEntity(const Entity entity)
    {
        m_entityList.RemoveEntity(entity);
    }

    std::shared_ptr<Entity> Scene::GetEntity(const std::string& tag)
    {
        Entity ent;

        m_entityList.m_Registry.view<Components::Tag>().each([&](auto entity, Components::Tag& tagComponent) {
            if (tagComponent.tag == tag)
                ent = m_entityList.GetEntity(static_cast<unsigned int>(entity));
        });

        return std::make_shared<Entity>(ent);
    }

    const Rendering::DebugScene& Scene::GetDebugScene() const
    {
        return m_debugScene;
    }

    bool Scene::IsDebugEnabled() const
    {
        return m_debugEnabled;
    }

    PhysicsScene& Scene::GetPhysicsScene()
    {
        return m_physicsScene;
    }

    namespace fs = std::filesystem;

    void Scene::Serialize(const std::string& folder) const
    {
        std::string temp = folder;
        std::replace(temp.begin(), temp.end(), '/', '\\'); // replace / with \\ for filesystem

        const std::string folderPath = "\\" + temp + "\\" + m_name;

        fs::path currentPath = fs::current_path();

        try
        {
            currentPath += folderPath;
            fs::create_directories(currentPath); // creates directory for scene if it doesnt exist
        }
        catch (...)
        {
            // directory already exists
        }

        const std::string FinalFolder = folder + "/" + m_name;
        m_entityList.Serialize(FinalFolder + "/EntityList.json");

        //-- Serialize Scene info
        std::ofstream output(FinalFolder + "/Scene.json");

        if (output.good())
        {
            cereal::JSONOutputArchive json(output);

            json(cereal::make_nvp("Scene Name", m_name));
            json(cereal::make_nvp("Script Name", m_script.path)); // TODO see if this works
            json(cereal::make_nvp("camera", m_cam));
        }

        std::ofstream globalOutput(FinalFolder + "/Globals.json");

        if (globalOutput.good())
        {
            cereal::JSONOutputArchive json(globalOutput);

            // SolTableData globals;
            // ConvertSolToMap(sceneGlobals, globals);
            // json(CEREAL_NVP(globals));
        }
    }

    void Scene::Deserialize(const std::string& folder)
    {
        std::ifstream globalInput(folder + "/Globals.json");

        if (globalInput.good())
        {
            // cereal::JSONInputArchive json(globalInput);

            // SolTableData globals;
            // json(globals);

            // ConvertMapToSol(lua, globals, sceneGlobals);
        }

        m_entityList.Deserialize(folder + "/EntityList.json");

        // System::Script_Deserialize(lua, entityList, this);

        std::ifstream input(folder + "/Scene.json");

        if (input.good())
        {
            cereal::JSONInputArchive json(input);
            json(m_name);
            json(m_script.path); // TODO see if this works
            json(m_cam);
        }
    }

}; // namespace Vakol
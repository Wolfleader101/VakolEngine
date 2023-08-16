#include "Scene.hpp"

#include <Controller/Camera.hpp>
#include <Model/Components.hpp>
#include <Model/Entity.hpp>
#include <algorithm>
#include <cereal/archives/json.hpp>
#include <filesystem>

#include "SolSerialize.hpp"
#include "System.hpp"

namespace Vakol::Controller
{
    Scene::Scene(const std::string& name, LuaScript& script, const std::shared_ptr<Physics::ScenePhysics>& SP)
        : scenePhysics(SP), m_script(std::move(script)), m_name(name), m_cam(Math::Vec3(0.0f, 0.0f, 2.0f))
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

    Model::Entity Scene::CreateEntity(const std::string& tag)
    {
        auto ent = m_entityList.CreateEntity();

        ent.GetComponent<Model::Components::Tag>().tag = tag;

        if (!ent.GetComponent<Model::Components::GUID>().id.isValid())
            ent.GetComponent<Model::Components::GUID>().GenNewGUID();

        return ent;
    }

    void Scene::DestroyEntity(const Model::Entity entity)
    {
        m_entityList.RemoveEntity(entity);
    }

    void Scene::Update(const Time& time, const std::shared_ptr<View::Renderer>& renderer)
    {
        scenePhysics->Update(time);

        System::Drawable_Update(m_cam, time, renderer);

        m_cam.Update();
    }

    std::shared_ptr<Model::Entity> Scene::GetEntity(const std::string& tag)
    {
        Model::Entity ent;

        m_entityList.m_Registry.view<Model::Components::Tag>().each(
            [&](auto entity, Model::Components::Tag& tagComponent) {
                if (tagComponent.tag == tag)
                    ent = m_entityList.GetEntity(static_cast<unsigned int>(entity));
            });

        return std::make_shared<Model::Entity>(ent);
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

        System::Physics_SerializationPrep();
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

        System::BindScene(*this);
        System::Drawable_Init();
        System::Physics_Init();
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

}; // namespace Vakol::Controller
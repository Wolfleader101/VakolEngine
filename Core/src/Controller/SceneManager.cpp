#include "SceneManager.hpp"

#include <memory>
#include "Controller/Physics/PhysicsPool.hpp"
#include "Controller/Physics/ScenePhysics.hpp"

namespace Vakol::Controller
{
    SceneManager::SceneManager(ScriptEngine& scriptEngine)
        : m_scriptEngine(scriptEngine)
    { }

    SceneManager::~SceneManager()
    {
    }

    Scene& SceneManager::GetCurrentScene()
    {
        if(m_scenes.empty()) 
        {
            std::string str = "No scenes in scene manager.";
            ThrowRuntime(str);
        }

        if(!m_currentScene)
        {
            std::string str = "Current scene not set.";
            ThrowRuntime(str);
        }

        return *m_currentScene;
    }

    void SceneManager::SetCurrentScene(const std::string& name)
    {
        if(!m_currentScene && (*m_currentScene).getName() == name) return;

        m_currentScene = &GetScene(name);
        
    }

    Scene& SceneManager::GetScene(const std::string& name) //slow but adds flexiblility
    {
        if(m_scenes.find(name) != m_scenes.end()) return m_scenes.at(name);

        ThrowRuntime("Scene with name " + name + " not found.");
    }


    void SceneManager::CreateScene(const std::string& name, const std::string& scriptName)
    {
        if(m_scenes.find(name) != m_scenes.end()) ThrowRuntime("Scene with name " + name + " already exists.");

        m_scenes.emplace(name, Scene(name, 
                                    m_scriptEngine.CreateScript("scripts/" + scriptName), 
                                    std::make_shared<Physics::ScenePhysics>(Physics::PhysicsPool::CreatePhysicsWorld())
                                    )
                        );
        
    }

    void SceneManager::RemoveScene(const std::string& name)
    {
        if(m_scenes.find(name) == m_scenes.end()) 
            ThrowRuntime("Scene with name " + name + " not found.");


        if(m_currentScene == &m_scenes.at(name)) m_currentScene = nullptr;

        m_scenes.erase(name);

    }

    void SceneManager::Update(Time& time, const std::shared_ptr<View::Renderer>& renderer)
    {
        time.accumulator += time.deltaTime;

        while (time.accumulator >= time.tickRate)
        {
            // eventually need to do something like m_scenemanager.GetCurrentScene().GetScript()

            // TODO set the current scene globally, eventually want to move this elsewhere
            m_scriptEngine.SetGlobalVariable("scene", m_currentScene);

            if (!m_currentScene->initialized) {
                m_scriptEngine.InitScript(m_currentScene->GetScript());

                m_currentScene->Init();
            }

            m_currentScene->GetEntityList().GetRegistry().view<LuaScript>().each(
                [&](auto& script) { m_scriptEngine.TickScript(script); });

            m_scriptEngine.TickScript(m_currentScene->GetScript());
            

            // Decrease the accumulated time
            time.accumulator -= time.tickRate;
        }


        if(!m_currentScene) ThrowRuntime("Current scene not set. Can not update Scene.");

        renderer->UpdateData(m_currentScene->GetCamera());

        // TODO set the current scene globally, eventually want to move this elsewhere
        m_scriptEngine.SetGlobalVariable("scene", m_currentScene);

        m_currentScene->GetEntityList().GetRegistry().view<LuaScript>().each(
            [&](auto& script) { m_scriptEngine.UpdateScript(script); });

        m_scriptEngine.UpdateScript(m_currentScene->GetScript());

        m_currentScene->Update(time, renderer);
    }


    bool SceneManager::operator!() const
    {
        bool empty = m_scenes.empty();
        bool currentNull = !m_currentScene;

        return empty || currentNull;
    }

    void ThrowRuntime(const std::string& str) //probably should be its own class but I'm lazy
    {
        VK_CRITICAL(str);
        throw std::runtime_error(str);
    }

}
#include "SceneManager.hpp"

#include <memory>

#include "Controller/Physics/PhysicsPool.hpp"
#include "Controller/Physics/ScenePhysics.hpp"

namespace Vakol 
{
    using Scene = Controller::Scene;

    SceneManager::SceneManager(ScriptEngine& scriptEngine) : m_scriptEngine(scriptEngine) {}

    SceneManager::~SceneManager()
    {
        for (auto& scene : m_scenes)
        {
            delete scene.second;
        }
        m_currentScene = nullptr;
    }

    Scene& SceneManager::GetCurrentScene() {

        if (m_scenes.empty()) 
            ThrowRuntime("No scenes in scene manager.");

        if (!m_currentScene)
            ThrowRuntime("Current scene not set.");


        return *m_currentScene;
    }

    void SceneManager::SetCurrentScene(const std::string& name) {
        if (!m_currentScene && (*m_currentScene).getName() == name) return;

        m_currentScene = &GetScene(name);
    }

    Scene& SceneManager::GetScene(const std::string& name)  // slow but adds flexiblility
    {
        if (m_scenes.find(name) != m_scenes.end()) return *m_scenes.at(name);

        ThrowRuntime("Scene with name " + name + " not found.");
    }

    void SceneManager::CreateScene(const std::string& name, const std::string& scriptName) {
        if (m_scenes.find(name) != m_scenes.end()) VK_ERROR("Scene with name: {0} already exists. Skipping...", name);

        auto script = m_scriptEngine.CreateScript("scripts/" + scriptName);
        m_scenes.emplace(name,
                         new Scene(name, script,
                               std::make_shared<Controller::Physics::ScenePhysics>(Controller::Physics::PhysicsPool::CreatePhysicsWorld())));
        
        if(m_scenes.size() == 1) m_currentScene = m_scenes.at(name);
    }

    void SceneManager::RemoveScene(const std::string& name) {
        if (m_scenes.find(name) == m_scenes.end()) ThrowRuntime("Scene with name " + name + " not found.");

        if (m_currentScene == m_scenes.at(name)) m_currentScene = nullptr;

        delete m_scenes.at(name);
        m_scenes.erase(name);
    }

    bool SceneManager::operator!() const {
        bool empty = m_scenes.empty();
        bool currentNull = !m_currentScene;

        return empty || currentNull;
    }

    void SceneManager::ThrowRuntime(const std::string& str) const  // probably should be its own class but I'm lazy
    {
        VK_CRITICAL(str);
        throw std::runtime_error(str);
    }

}  // namespace Vakol::Controller
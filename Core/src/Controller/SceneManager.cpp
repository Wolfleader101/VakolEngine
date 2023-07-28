#include "SceneManager.hpp"

#include <memory>

#include "Controller/Physics/PhysicsPool.hpp"
#include "Controller/Physics/ScenePhysics.hpp"

namespace Vakol::Controller {
    SceneManager::SceneManager(ScriptEngine& scriptEngine) : m_scriptEngine(scriptEngine) {}

    SceneManager::~SceneManager() {}

    Scene& SceneManager::GetCurrentScene() {
        if (m_scenes.empty()) {
            std::string str = "No scenes in scene manager.";
            ThrowRuntime(str);
        }

        if (!m_currentScene) {
            std::string str = "Current scene not set.";
            ThrowRuntime(str);
        }

        return *m_currentScene;
    }

    void SceneManager::SetCurrentScene(const std::string& name) {
        if (!m_currentScene && (*m_currentScene).getName() == name) return;

        m_currentScene = &GetScene(name);
    }

    Scene& SceneManager::GetScene(const std::string& name)  // slow but adds flexiblility
    {
        if (m_scenes.find(name) != m_scenes.end()) return m_scenes.at(name);

        ThrowRuntime("Scene with name " + name + " not found.");
    }

    void SceneManager::CreateScene(const std::string& name, const std::string& scriptName) {
        if (m_scenes.find(name) != m_scenes.end()) ThrowRuntime("Scene with name " + name + " already exists.");

        m_scenes.emplace(name,
                         Scene(name, m_scriptEngine.CreateScript("scripts/" + scriptName),
                               std::make_shared<Physics::ScenePhysics>(Physics::PhysicsPool::CreatePhysicsWorld())));
    }

    void SceneManager::RemoveScene(const std::string& name) {
        if (m_scenes.find(name) == m_scenes.end()) ThrowRuntime("Scene with name " + name + " not found.");

        if (m_currentScene == &m_scenes.at(name)) m_currentScene = nullptr;

        m_scenes.erase(name);
    }

    bool SceneManager::operator!() const {
        bool empty = m_scenes.empty();
        bool currentNull = !m_currentScene;

        return empty || currentNull;
    }

    void ThrowRuntime(const std::string& str)  // probably should be its own class but I'm lazy
    {
        VK_CRITICAL(str);
        throw std::runtime_error(str);
    }

}  // namespace Vakol::Controller
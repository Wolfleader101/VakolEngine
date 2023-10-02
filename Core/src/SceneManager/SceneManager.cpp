#include "SceneManager/SceneManager.hpp"

#include <memory>

namespace Vakol
{
    SceneManager::SceneManager(ScriptEngine& scriptEngine)
        : m_scriptEngine(scriptEngine) 
    {
    }

    SceneManager::~SceneManager()
    {
        for (auto& scene : m_scenes)
        {
            delete scene.second;
        }
        m_activeScene = nullptr;
    }

    Scene& SceneManager::GetActiveScene()
    {
        if (m_scenes.empty())
            ThrowRuntime("No scenes in scene manager.");

        if (!m_activeScene)
            ThrowRuntime("There is no active scene");

        return *m_activeScene;
    }

    void SceneManager::ChangeActiveScene(const std::string& name)
    {
        if (!m_activeScene && (*m_activeScene).getName() == name)
            return;

        m_nextScene = &GetScene(name);
    }

    Scene& SceneManager::GetScene(const std::string& name) // slow but adds flexiblility
    {
        if (m_scenes.find(name) != m_scenes.end())
            return *m_scenes.at(name);

        ThrowRuntime("Scene with name " + name + " not found.");
    }

    void SceneManager::CreateScene(const std::string& name, const std::string& scriptName)
    {
        if (m_scenes.find(name) != m_scenes.end())
            VK_ERROR("Scene with name: {0} already exists. Skipping...", name);

        auto script = m_scriptEngine.CreateScript("scripts/" + scriptName);

        m_scenes.emplace(name, new Scene(name, script));

        if (m_scenes.size() == 1)
            m_activeScene = m_scenes.at(name);
    }

    void SceneManager::RemoveScene(const std::string& name)
    {
        if (m_scenes.find(name) == m_scenes.end())
            ThrowRuntime("Scene with name " + name + " not found.");

        if (m_activeScene == m_scenes.at(name))
            m_activeScene = nullptr;

        delete m_scenes.at(name);
        m_scenes.erase(name);
    }

    void SceneManager::Update()
    {
        if (m_nextScene != nullptr)
        {
            m_activeScene = m_nextScene;
            m_nextScene = nullptr;
            m_sceneChanged = true;
            m_counter = 0;
        }

        if (m_sceneChanged)
        {
            m_counter++;
            if (m_counter >= 3)
            {
                m_sceneChanged = false;
            }
        }

        m_scriptEngine.SetGlobalVariable("scene", m_activeScene);

        if (!m_activeScene->initialized)
        {
            m_scriptEngine.InitScript(m_activeScene->GetScript());
            m_activeScene->initialized = true;
        }
    }
    bool SceneManager::SceneChanged() const
    {
        return m_sceneChanged;
    }

    bool SceneManager::operator!() const
    {
        bool empty = m_scenes.empty();
        bool currentNull = !m_activeScene;

        return empty || currentNull;
    }

    void SceneManager::ThrowRuntime(const std::string& str) const // probably should be its own class but I'm lazy
    {
        VK_CRITICAL(str);
        throw std::runtime_error(str);
    }

} // namespace Vakol
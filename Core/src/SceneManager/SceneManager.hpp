#pragma once

#include "Controller/Scene.hpp"
#include "View/Renderer/Renderer.hpp"

namespace Vakol 
{
    using Scene = Controller::Scene;
    /**
     * @class SceneManager
     *
     * @brief Class representing the scene manager in the application.
     */
    class SceneManager 
    {
       

       public:
        /**
         * @brief Construct a new Scene Manager object.
         *
         * @param scriptEngine The script engine.
         */
        SceneManager(ScriptEngine& scriptEngine);

        /**
         * @brief Destroy the Scene Manager object.
         */
        ~SceneManager();

        /**
         * @brief Get the current scene.
         *
         * @return Scene& The current scene.
         */
        Scene& GetCurrentScene();

        /**
         * @brief Set the current scene.
         *
         * @param scene The new current scene.
         */
        void SetCurrentScene(const std::string& name);

        /**
         * @brief Get the scene with the given name. Doesn't make it the current scene. Use SetCurrentScene for that.
         *
         * @param name The name of the scene to get.
         * @return Scene& The scene with the given name.
         */
        Scene& GetScene(const std::string& name);

        /**
         * @brief Create a Scene object
         *
         * @param name Name of Scene (unique)
         * @param scriptName Script name
         *
         */
        void CreateScene(const std::string& name, const std::string& scriptName);

        /**
         * @brief Remove a scene from the scene manager.
         *
         * @param scene The scene to remove.
         */
        void RemoveScene(const std::string& name);

        /**
         * @brief Used to check if scene manager is valid to use.
         *        Checks if empty, and if current scene is set.
         *
         * @return true
         * @return false
         */
        bool operator!() const;

       private:
        void ThrowRuntime(const std::string& str);

        Scene* m_currentScene;

        /**
         * @brief The map of scenes.
         */
        std::unordered_map<std::string, Scene> m_scenes;

        /**
         * @brief The script engine.
         */
        ScriptEngine& m_scriptEngine;
    };
}  // namespace Vakol::Controller
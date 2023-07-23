#pragma once

#include "Scene.hpp"

#include "View/Renderer/Renderer.hpp"

namespace Vakol::Controller
{
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
        void SetCurrentScene(unsigned int index);

        /**
         * @brief Get the Scene object at the given index.
         * 
         * @param index 
         * @return Scene& scene at the given index
         */
        Scene& GetScene(unsigned int index);

        /**
         * @brief Get the scene with the given name.
         *
         * @param name The name of the scene to get.
         * @return Scene& The scene with the given name.
         */
        Scene& GetScene(const std::string& name);

        /**
         * @brief Add a scene to the scene manager.
         *
         * @param scene The scene to add.
         */
        unsigned int CreateScene();

        /**
         * @brief Remove a scene from the scene manager.
         *
         * @param scene The scene to remove.
         */
        void RemoveScene(unsigned int index);

        /**
         * @brief Update the current scene.
         *
         * @param time The time information for the update.
         * @param renderer The shared pointer to the renderer.
         */
        void Update(const Time& time, const std::shared_ptr<Vakol::View::Renderer>& renderer);

    private:
        /**
         * @brief The current scene.
         */
        unsigned int m_currentScene;

        /**
         * @brief The map of scenes.
         */
        std::vector<Scene> m_scenes;

        /**
         * @brief The script engine.
         */
        ScriptEngine& m_scriptEngine;
    };
}
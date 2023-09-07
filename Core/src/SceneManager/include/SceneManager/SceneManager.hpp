#pragma once

#include "Physics/PhysicsEngine.hpp"
#include "Scene.hpp"
#include "Scripting/ScriptEngine.hpp"

namespace Vakol
{
    // using Scene = Scene;
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
         * @param physicsEngine The physics engine.
         */
        SceneManager(ScriptEngine& scriptEngine, PhysicsEngine& physicsEngine);

        /**
         * @brief Destroy the Scene Manager object.
         */
        ~SceneManager();

        /**
         * @brief Get the current scene.
         *
         * @return Scene& of active scene
         */
        Scene& GetActiveScene();

        /**
         * @brief Change the active current scene.
         *
         * @param name of the new scene
         */
        void ChangeActiveScene(const std::string& name);

        /**
         * @brief Get a scene with the given name.
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
         */
        void CreateScene(const std::string& name, const std::string& scriptName);

        /**
         * @brief Remove a scene from the scene manager.
         *
         * @param scene The scene to remove.
         */
        void RemoveScene(const std::string& name);

        /**
         * @brief Update the scene manager. Internally will change to the next scene if its been set
         *
         */
        void Update();

        bool SceneChanged() const;

        /**
         * @brief Used to check if scene manager is valid to use.
         *        Checks if empty, and if current scene is set.
         *
         * @return true
         * @return false
         */
        bool operator!() const;

      private:
        void ThrowRuntime(const std::string& str) const;

        Scene* m_activeScene;
        Scene* m_nextScene;
        bool m_sceneChanged;
        int m_counter;

        /**
         * @brief The map of scenes.
         */
        std::unordered_map<std::string, Scene* const> m_scenes;

        /**
         * @brief The script engine.
         */
        ScriptEngine& m_scriptEngine;

        /**
         * @brief the physics engine
         *
         */
        PhysicsEngine& m_physicsEngine;
    };
} // namespace Vakol
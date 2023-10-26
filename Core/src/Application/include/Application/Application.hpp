#pragma once

#include <memory>

#include "GUI/GUIWindow.hpp"
#include "Input/Input.hpp"
#include "Physics/PhysicsEngine.hpp"
#include "SceneManager/SceneManager.hpp"
#include "Scripting/ScriptEngine.hpp"
#include "Time/Time.hpp"
#include "Window/Events/KeyEvent.hpp"
#include "Window/Events/WindowEvent.hpp"
#include "Window/Window.hpp"

#include "LayerManager/LayerManager.hpp"
#include "Utils/Layer.hpp"

namespace Vakol
{

    struct GameConfig;

    enum class GameState
    {
        Running,
        Paused
    };

    enum class SystemFlag : int
    {
        Scripting = 1 << 0, // Same as 1
        Physics = 1 << 1,   // Same as 2
        Rendering = 1 << 2  // Same as 4
    };

    /**
     * @class Application
     * @brief The main application class to launch the game engine
     *
     */
    class Application
    {
      public:
        /**
         * @brief Construct a new Application object
         * @param configPath path to the config file
         *
         */
        Application(const std::string& configPath = "scripts/game_config.lua");

        /**
         * @brief destructor of the application
         */
        ~Application() = default;

        /**
         * @brief run the application
         */
        void Run();

        /**
         * @brief handle window events
         * @param ev event to handle
         */
        void OnEvent(Event& ev);

        /**
         * @brief check if application is still running
         * @return true if running
         */
        bool IsRunning() const;

        /**
         * @brief Get the Width of window
         *
         * @return const int
         */
        int GetWidth() const
        {
            return m_window->GetWidth();
        }

        /**
         * @brief Get the Height of window
         *
         * @return const int
         */
        int GetHeight() const
        {
            return m_window->GetHeight();
        }

        /**
         * @brief Set the running state of the game.
         *
         * @param run Flag indicating whether the game is running.
         */
        void SetRunning(const bool run)
        {
            m_running = run;
        }

        /**
         * @brief Get the time information.
         *
         * @return const Time& The time information.
         */
        const Time& GetTime() const
        {
            return m_time;
        }

        /**
         * @brief Get the input information.
         *
         * @return const Input& The input information.
         */
        const Input& GetInput() const
        {
            return m_input;
        }

        /**
         * \brief Get the physics engine.
         * \return the physics engine
         */
        PhysicsEngine& GetPhysicsEngine()
        {
            return m_physicsEngine;
        }

        /**
         * @brief Get the Scene Manager object
         *
         * @return SceneManager&
         */
        SceneManager& GetSceneManager()
        {
            return m_sceneManager;
        }

        /**
         * @brief Set the Active Mouse object
         *
         * @param active mouse is active or not
         */
        void SetActiveMouse(bool active);

        /**
         * @brief Register Lua scripting.
         */
        void RegisterLua();

        const std::shared_ptr<Window> GetWindow() const;

        /**
         * @brief Push a layer to the top of the stack
         *
         * @param layer to push
         * @param flags to subscribe to
         */
        void PushLayer(std::shared_ptr<Layer> layer, LayerSubscription flags);

        /**
         * @brief Pop the top layer
         *
         */
        void PopLayer();

        /**
         * @brief Get the Game State object
         *
         * @return GameState
         */
        GameState GetGameState() const;

        /**
         * @brief Set the Game State object
         *
         * @param state to set
         */
        void SetGameState(GameState state);

        /**
         * @brief toggle a system on or off
         *
         * @param system to toggle
         */
        void ToggleSystem(SystemFlag system);

        /**
         * @brief check if a system is active
         *
         * @param system to check
         * @return true if on
         * @return false if off
         */
        bool IsSystemActive(SystemFlag system) const;

      private:
        /**
         * @brief Init function for application
         *
         */
        void Init(GameConfig& config);

        /**
         * @brief on window close event
         * @param ev event of windowClose
         * @return true if it was sucessful
         */
        bool OnWindowClose(WindowCloseEvent& ev);

        /**
         * @brief Handle the window resize event.
         *
         * @param ev The WindowResizeEvent.
         * @return bool True if the event was handled, false otherwise.
         */
        bool OnWindowResize(const WindowResizeEvent& ev);

        /**
         * @brief on keyboard press event
         * @param kev on key pressed event
         * @return true if successful
         */
        bool OnKeyPressed(KeyPressedEvent& kev);

        /**
         * @brief Handle the key released event.
         *
         * @param kev The KeyReleasedEvent.
         * @return bool True if the event was handled, false otherwise.
         */
        bool OnKeyReleased(KeyReleasedEvent& kev);

        /**
         * @brief Handle the mouse moved event.
         *
         * @param ev The MouseMovedEvent.
         * @return bool True if the event was handled, false otherwise.
         */
        bool OnMouseMoved(MouseMovedEvent& ev);

        /**
         * @brief Handle the mouse button pressed event.
         *
         * @param mev The MouseButtonPressedEvent.
         * @return bool True if the event was handled, false otherwise.
         */
        bool OnMouseButtonPressed(MouseButtonPressedEvent& mev);

        /**
         * @brief Handle the mouse button released event.
         *
         * @param mev The MouseButtonReleasedEvent.
         * @return bool True if the event was handled, false otherwise.
         */
        bool OnMouseButtonReleased(MouseButtonReleasedEvent& mev);

        void OnCollision(RigidBody& rb1, RigidBody& rb2);

        /**
         * @brief load the game config
         *
         * @return std::optional<GameConfig> return the config
         */
        std::optional<GameConfig> LoadConfig(const std::string& configPath);

        /**
         * @brief the window of the application
         */
        std::shared_ptr<Window> m_window;

        /**
         * @brief the time data of engine
         */
        Time m_time;

        /**
         * @brief Script engine instance
         *
         */
        ScriptEngine m_scriptEngine;

        /**
         * @brief physics engine instance
         *
         */
        PhysicsEngine m_physicsEngine;

        /**
         * @brief Scene manager instance
         *
         */
        SceneManager m_sceneManager;

        LayerManager m_layerManager;

        /**
         * @brief if the app should be running
         */
        bool m_running;

        /**
         * @brief state of the game
         *
         */
        GameState m_gameState;

        int m_activeSystems;

        /**
         * @brief the gui window
         */
        GUIWindow m_gui;

        /**
         * @brief the input data of the engine
         */
        Input m_input;
    };
} // namespace Vakol
#pragma once

#include <View/GUI/GUIWindow.hpp>
#include <View/Renderer/Renderer.hpp>
#include <View/Window/Window.hpp>
#include <memory>

#include "Events/KeyEvent.hpp"
#include "Events/WindowEvent.hpp"
#include "Time.hpp"

// just using this to test if ecs hooked in properly
#include <Controller/EntityList.hpp>
#include <Controller/LuaState.hpp>
#include <Model/Entity.hpp>
#include <Model/GameConfig.hpp>

#include "Input.hpp"
#include "Scene.hpp"

namespace Vakol::Controller {

    /**
     * @class Application
     * @brief The main application class to launch the game engine, expected use is a singleton
     *
     */
    class Application {
       public:
        /**
         * @brief Construct a new Application object
         *
         */
        Application();

        void Init();

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
        int GetWidth() const { return m_window->GetWidth(); }

        /**
         * @brief Get the Height of window
         *
         * @return const int
         */
        int GetHeight() const { return m_window->GetHeight(); }

        /**
         * @brief Add a scene with a script name and an optional scene name.
         *
         * @param scriptName The script name of the scene.
         * @param scene_name The name of the scene (optional).
         */
        void AddScene(const std::string& scriptName, const std::string& scene_name = "");

        /**
         * @brief Get a scene by its name.
         *
         * @param sceneName The name of the scene.
         * @return Scene& A reference to the scene.
         */
        Scene& GetScene(const std::string& sceneName);

        /**
         * @brief Set the running state of the game.
         *
         * @param run Flag indicating whether the game is running.
         */
        void SetRunning(const bool run) { m_running = run; }

        /**
         * @brief Get the time information.
         *
         * @return const Time& The time information.
         */
        const Time& GetTime() const { return m_time; }

        /**
         * @brief Get the input information.
         *
         * @return const Input& The input information.
         */
        const Input& GetInput() const { return m_input; }

        /**
         * @brief Register Lua scripting.
         */
        void RegisterLua();

    private:
        /**
         * @brief
         *
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
        bool OnWindowResize(const WindowResizeEvent& ev) const;

        /**
         * @brief on keyboard press event
         * @param kev on key pressed event
         * @return true if successful
         */
        bool Application::OnKeyPressed(KeyPressedEvent& kev);

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

        /**
         * @brief load the game config
         *
         * @return std::optional<GameConfig> return the config
         */
        std::optional<Model::GameConfig> LoadConfig();

        /**
         * @brief the window of the application
         */
        std::shared_ptr<View::Window> m_window;

        /**
         * @brief the time data of engine
         */
        Time m_time;

        /**
         * @brief the renderer instance of the class
         */
        std::shared_ptr<View::Renderer> m_renderer;

        /**
         * @brief holds the lua state
         *
         */
        std::shared_ptr<LuaState> lua;


        /**
         * @brief holds the scenes
         */
        std::vector<Scene> scenes;

        /**
         * @brief if the app should be running
         */
        bool m_running = true;

        /**
         * @brief the gui window
         */
        View::GUIWindow m_gui;

        /**
         * @brief the input data of the engine
         */
        Input m_input;

    };
}

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

        void AddScene(const std::string& scriptName, const std::string& scene_name = "");
        Scene& GetScene(const std::string& sceneName);

        void SetRunning(const bool run) { m_running = run; }

        const Time& GetTime() const { return m_time; }

        const Input& GetInput() const { return m_input; }

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

        bool OnWindowResize(const WindowResizeEvent& ev) const;

        /**
         * @brief on keyboard press event
         * @param kev on key pressed event
         * @return true if successful
         */
        bool Application::OnKeyPressed(KeyPressedEvent& kev);

        bool OnKeyReleased(KeyReleasedEvent& kev);
        bool OnMouseMoved(MouseMovedEvent& ev);

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
        LuaState lua;

        std::vector<Scene> scenes;

        /**
         * @brief if the app should be running
         */
        bool m_running = true;

        View::GUIWindow m_gui;

        Input m_input;

    };  // namespace Vakol::Controller
}  // namespace Vakol::Controller

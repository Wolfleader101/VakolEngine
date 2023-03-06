#pragma once

#include <View/Renderer/Renderer.hpp>
#include <View/Window/Window.hpp>
#include <memory>

#include "Events/Event.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/WindowEvent.hpp"
#include "Time.hpp"

// just using this to test if ecs hooked in properly
#include <Controller/EntityList.hpp>
#include <Controller/LuaState.hpp>
#include <Model/Entity.hpp>
#include <Model/GameConfig.hpp>

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
        ~Application();

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
        const int GetWidth() const { return m_window->GetWidth(); }

        /**
         * @brief Get the Height of window
         *
         * @return const int
         */
        const int GetHeight() const { return m_window->GetHeight(); }

       private:
        /**
         * @brief on window close event
         * @param ev event of windowClose
         * @return true if it was sucessful
         */
        bool OnWindowClose(WindowCloseEvent& ev);

        bool OnWindowResize(WindowResizeEvent& ev);

        /**
         * @brief on keyboard press event
         * @param kev on key pressed event
         * @return true if successful
         */
        bool Application::OnKeyPressed(KeyPressedEvent& kev);

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
         * @brief the entity list of engine
         */
        // Vakol::Model::ECS::EntityList m_entityList;  // to be moved to game/scene but for now it all works

        /**
         * @brief the renderer instance of the class
         */
        std::shared_ptr<View::Renderer> m_renderer;

        /**
         * @brief holds the lua state
         *
         */
        LuaState lua;

        /**
         * @brief if the app should be running
         */
        bool m_running = true;
    };  // namespace Vakol::Controller
}  // namespace Vakol::Controller

#pragma once
#include <glad/glad.h>

#include <functional>
#include <string>

#include "GLFW/glfw3.h"

namespace Vakol
{
    class Event;
}

namespace Vakol
{

    /**
     * @class Window
     * @brief window class containing window data
     */
    class Window
    {
      public:
        using EventCallbackFn = std::function<void(Event&)>;

        /**
         * @brief window constructor
         * @param title of window
         * @param width of window
         * @param height of window
         */
        Window(std::string title, int width, int height);

        /**
         * @brief Destroy the Window object
         *
         */
        ~Window();

        /**
         * @brief on window update
         */
        void OnUpdate() const;

        /**
         * @brief get the GLFW window
         * @return
         */
        GLFWwindow* GetWindow() const;

        /**
         * @brief set the event callback function
         * @param callback to set
         */
        void SetEventCallback(const EventCallbackFn& callback);

        /**
         * @brief get the width of the window
         * @return width of the window
         */
        const int& GetWidth() const
        {
            return m_width;
        }

        /**
         * @brief get the height of the window
         * @return height of window
         */
        const int& GetHeight() const
        {
            return m_height;
        }

      private:
        /**
         * @brief glfw window ptr
         */
        GLFWwindow* m_window;

        /**
         * @brief event callback fn
         */
        EventCallbackFn m_eventCallback;

        /**
         * @brief title of window
         */
        std::string m_title;

        /**
         * @brief width of window
         */
        int m_width;

        /**
         * @brief height of window
         */
        int m_height;
    };
} // namespace Vakol
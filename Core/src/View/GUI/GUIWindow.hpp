#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

namespace Vakol::View 
{
    /**
     * @class GUIWindow
     * @brief GUIWindow class containing UI data
     */
    class GUIWindow 
    {
        public:
            /**
            * @brief Basic Constructor
            */
            GUIWindow();
            /**
            * @brief Initialises the GUI Element
            * 
            * @param window The GLFW window the UI will be inside of
            */
            void InitUI(GLFWwindow* window);
            /**
            * @brief Creates a new frame for the window
            */
            void CreateNewFrame();
            /**
            * @brief Starts the creation of a GUI window
            * 
            * @param windowName The name of the window (Will also be the title given to the window)
            * @param width The width of the window
            * @param height The height of the window
            */
            void StartWindowCreation(const char* windowName, float width, float height);
            /**
            * @brief Adds a button to a given window
            *
            * @param buttonName The name of the button
            * @param width The width of the button
            * @param height The height of the button
            */
            void AddButton(const char* buttonName, float width, float height);
            /**
            * @brief Ends the creation of a GUI window
            */
            void EndWindowCreation();
            /**
            * @brief A basic deconstructor for the GUI
            */
            ~GUIWindow();
        private:
    };
}  // namespace Vakol::View
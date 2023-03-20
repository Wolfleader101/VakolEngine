#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <functional> //Gives acces to std::function

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
            void StartWindowCreation(std::string& windowName, float width, float height, float xPosition, float yPosition);
            /**
            * @brief Allows you to start drawing from a specific position inside the window
            *
            * @param xPosition Sets the X-Position of the drawing cursor
            * @param yPosition Sets the Y-Position of the drawing cursor
            */
            void SetPositionInsideWIndow(float xPosition, float yPosition);
            /**
            * @brief Adds a button to a given window
            *
            * @param buttonName The name of the button
            * @param width The width of the button
            * @param height The height of the button
            * @param inputFunction A function passed into the button to be run when the button is pressed
            */
            void AddButton(std::string& buttonName, float width, float height, std::function<void()> inputFunction);
            /**
             * @brief Adds a checkbox to a given window
             *
             * @param checkboxName The name of the checkbox
             * @param checkBoxValue The boolean value of the checkbox
             */
            void AddCheckbox(std::string& checkboxName, bool &checkBoxValue);
            /**
             * @brief Adds an Integer Slider to the window
             *
             * @param sliderName The name of the slider
             * @param sliderValue The parameter containing the current slider value
             * @param minValue The minimum value of the slider
             * @param maxValue the maximum value of the slider
             */
            void AddIntSlider(std::string& sliderName, std::unique_ptr<int> sliderValue, int minValue, int maxValue);
            /**
             * @brief Adds a Vecotr based Integer Slider to the window
             *
             * @param sliderName The name of the slider
             * @param arraySize The size of the given array
             * @param sliderValue The parameter containing the current slider value
             * @param minValue The minimum value of the slider
             * @param maxValue the maximum value of the slider
             */
            void AddVecIntSlider(std::string& sliderName, int *sliderValue, unsigned arraySize, int minValue, int maxValue);
            /**
             * @brief Adds a Float Slider to the window
             *
             * @param sliderName The name of the slider
             * @param sliderValue The parameter containing the current slider value
             * @param minValue The minimum value of the slider
             * @param maxValue the maximum value of the slider
             */
            void AddFloatSlider(std::string& sliderName, float& sliderValue, float minValue, float maxValue);
            /**
             * @brief Adds a Vecotr based Float Slider to the window
             *
             * @param sliderName The name of the slider
             * @param arraySize The size of the given array
             * @param sliderValue The parameter containing the current slider value
             * @param minValue The minimum value of the slider
             * @param maxValue the maximum value of the slider
             */
            void AddVecFloatSlider(std::string& sliderName, float* sliderValue, unsigned arraySize, float minValue, float maxValue);
            /**
            * @brief Ends the creation of a GUI window
            */
            void EndWindowCreation();
            /**
            * @brief A basic deconstructor for the GUI
            */
            ~GUIWindow();
        private:
            ImGuiWindowFlags windowFlags;
    };
}  // namespace Vakol::View
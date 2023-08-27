#pragma once

#include <imgui.h>

#include <functional>
#include <memory>
#include <string>

#include "Window/Window.hpp"

namespace Vakol
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
        void Init(const std::shared_ptr<Window>& window);
        /**
         * @brief Creates a new frame for the window
         */
        void CreateNewFrame() const;
        /**
         * @brief Changes the default font being used in Dear ImGui
         *
         * @param inputPath The path to the font file
         */
        void ChangeFontDefault(std::string inputPath) const;
        /**
         * @brief Ends the current frame
         */
        void EndFrame() const;

        /**
         * @brief Renders the GUI window
         */
        void Render() const;
        /**
         * @brief Returns the width of the display window
         */
        float DisplayWindowWidth() const;
        /**
         * @brief Returns the height of the display window
         */
        float DisplayWindowHeight() const;
        /**
         * @brief Starts the creation of a GUI window
         *
         * @param windowName The name of the window (Will also be the title given to the window)
         * @param centerX Whether the window should be centered on the x axis
         * @param centerY Whether the window should be centered on the y axis
         * @param width The width of the window
         * @param height The height of the window
         * @param xOffset The x position of the window will be offset by
         * @param yOffset The y position of the window will be offset by
         */
        void StartWindowCreation(const std::string& windowName, bool centerX, bool centerY, float width, float height,
                                 const float xOffset, float yOffset) const;
        /**
         * @brief Returns the FPS of the current window
         */
        float GetFramesPerSecond() const;
        /**
         * @brief An update function to call new frame
         *
         */
        void Update() const;
        /**
         * @brief Add a text string to the UI Window
         *
         * @param inputText The text to be set in the UI
         * @param centerX should center on X axis
         * @param centerY should center on Y axis
         * @param fontSize The size of the font
         * @param inputRed The red value of the text
         * @param inputGreen The green value of the text
         * @param inputBlue The blue value of the text
         * @param inputAlpha The alpha value of the text
         */
        void AddText(const std::string& inputText, const bool centerX, const bool centerY, const float fontSize,
                     const float inputRed, const float inputGreen, const float inputBlue, const float inputAlpha) const;

        void AddImage(unsigned int id, const ImVec2& imageSize, bool centerX, bool centerY) const;

        /**
         * @brief Adds a button to a given window
         *
         * @param buttonName The name of the button
         * @param width The width of the button
         * @param height The height of the button
         * @param inputFunction A function passed into the button to be run when the button is pressed
         */
        void AddButton(const std::string& buttonName, const bool centerX, const bool centerY, float width, float height,
                       const std::function<void()>& inputFunction) const;
        /**
         * @brief Adds a checkbox to a given window
         *
         * @param checkboxName The name of the checkbox
         * @param checkBoxValue The boolean value of the checkbox
         */
        static void AddCheckbox(const std::string& checkboxName, bool* checkBoxValue);
        /**
         * @brief Adds an Integer Slider to the window
         *
         * @param sliderName The name of the slider
         * @param sliderValue The parameter containing the current slider value
         * @param minValue The minimum value of the slider
         * @param maxValue the maximum value of the slider
         */
        void AddIntSlider(const std::string& sliderName, int* sliderValue, int minValue, int maxValue) const;
        /**
         * @brief Adds a Vector based Integer Slider to the window
         *
         * @param sliderName The name of the slider
         * @param size the size of an array
         * @param sliderValue The parameter containing the current slider value
         * @param minValue The minimum value of the slider
         * @param maxValue the maximum value of the slider
         */
        void AddVecIntSlider(const std::string& sliderName, int sliderValue[], const int size, const int minValue,
                             const int maxValue) const;
        /**
         * @brief Adds a Float Slider to the window
         *
         * @param sliderName The name of the slider
         * @param sliderValue The parameter containing the current slider value
         * @param minValue The minimum value of the slider
         * @param maxValue the maximum value of the slider
         */
        void AddFloatSlider(const std::string& sliderName, float* sliderValue, const float minValue,
                            const float maxValue) const;

        /**
         * @brief Adds a Vector based Float Slider to the window
         *
         * @param sliderName The name of the slider
         * @param size The size of the given array
         * @param sliderValue The parameter containing the current slider value
         * @param minValue The minimum value of the slider
         * @param maxValue the maximum value of the slider
         */
        void AddVecFloatSlider(const std::string& sliderName, float sliderValue[], const int size, const float minValue,
                               const float maxValue) const;
        /**
         * @brief Adds a new line in the same window
         */
        void SameLine() const;

        /**
         * @brief Sets the background style of the window
         *
         * @param inputRed The red value of the background color
         * @param inputGreen The green value of the background color
         * @param inputBlue The blue value of the background color
         * @param inputAlpha The alpha value of the background color
         */
        void WindowBackgroundStyle(const float inputRed, const float inputGreen, const float inputBlue,
                                   const float inputAlpha) const;

        /**
         * @brief Sets the rounding style of the window
         *
         * @param inputValue The value of the rounding style
         */
        void WindowRoundingStyle(const float inputValue) const;
        /**
         * @brief Ends the creation of a GUI window
         */
        void EndWindowCreation() const;
        /**
         * @brief A basic deconstructor for the GUI
         */
        ~GUIWindow();

      private:
        void SetAsContext() const;

        std::vector<ImFont*> fonts;  /**< Fonts used in the GUI window */
        bool is_initialised = false; /**< Flag indicating whether the GUI window is initialized */
        std::string scriptName;      /**< Name of the script */
        ImGuiContext* m_context;     /**< The context of the GUI window */
    };
} // namespace Vakol

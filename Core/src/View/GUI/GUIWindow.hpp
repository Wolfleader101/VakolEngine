#pragma once

#include <View/Window/Window.hpp>
#include <functional>  //Gives acces to std::function
#include <string>

#include <Controller/LuaState.hpp>
#include <imgui.h>

namespace Vakol::View {
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
    	void Init(const std::shared_ptr<Window>& window) const;
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
        void EndFrame() const;
        /**
         * @brief Starts the creation of a GUI window
         *
         * @param windowName The name of the window (Will also be the title given to the window)
         * @param width The width of the window
         * @param height The height of the window
         * @param x
         * @param y
         */
        void StartWindowCreation(const std::string& windowName, float width, float height, float x, float y) const;
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
         * @param centerX
         * @param centerY
         * @param fontSize The size of the font
         * @param inputRed The red value of the text
         * @param inputGreen The green value of the text
         * @param inputBlue The blue value of the text
         * @param inputAlpha The alpha value of the text
         */
        void AddText(const std::string& inputText, const bool centerX, const bool centerY, const float fontSize, const float inputRed, const float inputGreen, const float inputBlue, const float inputAlpha) const; 

        /**
         * \brief Adds an image to the UI Window
         * \param id
         * \param imageSize
         * \param centerX
         * \param centerY
         */
        void AddImage(unsigned int id, const ImVec2& imageSize, bool centerX, bool centerY) const;
        /**
         * @brief Adds a button to a given window
         *
         * @param buttonName The name of the button
         * @param width The width of the button
         * @param height The height of the button
         * @param inputFunction A function passed into the button to be run when the button is pressed
         */
    	void AddButton(const std::string& buttonName, float width, float height, const std::function<void()>& inputFunction) const;
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
    	void AddVecIntSlider(const std::string& sliderName, int sliderValue[], const int size, const int minValue, const int maxValue) const;
        /**
         * @brief Adds a Float Slider to the window
         *
         * @param sliderName The name of the slider
         * @param sliderValue The parameter containing the current slider value
         * @param minValue The minimum value of the slider
         * @param maxValue the maximum value of the slider
         */
    	void AddFloatSlider(const std::string& sliderName, float* sliderValue, const float minValue, const float maxValue) const;

    	/**
         * @brief Adds a Vector based Float Slider to the window
         *
         * @param sliderName The name of the slider
         * @param size The size of the given array
         * @param sliderValue The parameter containing the current slider value
         * @param minValue The minimum value of the slider
         * @param maxValue the maximum value of the slider
         */
    	void AddVecFloatSlider(const std::string& sliderName, float sliderValue[], const int size, const float minValue, const float maxValue) const;
        void WindowBackgroundStyle(const float inputRed, const float inputGreen, const float inputBlue, const float inputAlpha) const;
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
        std::vector<ImFont*> fonts;

        bool is_initialised = false;

        std::string scriptName;
    };
}  // namespace Vakol::View
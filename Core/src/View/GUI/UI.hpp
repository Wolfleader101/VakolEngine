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
     * @class UI
     * @brief UI class containing UI data
     */
    class UI 
    {
        public:
            UI();
            void InitUI(GLFWwindow* window);
            void CreateNewFrame();
            void CreateWindow(const char* windowName);
            ~UI();
        private:
            
    };
}  // namespace Vakol::View
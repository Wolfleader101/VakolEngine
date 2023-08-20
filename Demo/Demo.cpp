// Vakol_Engine.cpp : Defines the entry point for the application.

#include "Layers/MyGUILayer.hpp"
#include "Vakol/core.hpp"

int main()
{

    Vakol::PushLayer(std::make_shared<MyGUILayer>());

    Vakol::Run();

    return 0;
}

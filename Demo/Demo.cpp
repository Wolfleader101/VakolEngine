// Vakol_Engine.cpp : Defines the entry point for the application.
//

#include <Controller/Logger.hpp>
#include <Controller/LuaState.hpp>
#include <Controller/Singleton.hpp>
#include <core.hpp>
#include <iostream>

int main() 
{
    Vakol::Controller::Singleton<Vakol::Controller::LuaState>::GetInstance().RunFile("assets/test.lua");
    Vakol::Run();
    return 0;
}

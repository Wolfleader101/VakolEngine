// Vakol_Engine.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <core.hpp>

#include <Controller/Singleton.hpp>
#include <Controller/LuaState.hpp>
#include <Controller/Logger.hpp>


int main()
{
	Vakol::Init();
	VK_TRACE("Hello from Demo");

	Vakol::Controller::Singleton<Vakol::Controller::LuaState>::GetInstance().RunFile("assets/test.lua");
	return 0;
}

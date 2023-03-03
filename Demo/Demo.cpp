// Vakol_Engine.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <core.hpp>

#include <Controller/Singleton.hpp>
#include <Controller/LuaState.hpp>


int main()
{
	std::cout << "Hello from Demo" << std::endl;
	Vakol::Main();

	Vakol::Controller::Singleton<Vakol::Controller::LuaState>::GetInstance().RunFile("assets/test.lua");
	return 0;
}

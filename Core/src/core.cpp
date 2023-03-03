#include <iostream>
#include <Controller/Singleton.hpp>
#include <Controller/LuaState.hpp>
#include <Controller/Logger.hpp>

namespace Vakol {

	void Init()
	{
		// INITIALISE LOGGER
    	Controller::Logger::Init();

		VK_TRACE("Hello from core");

		Controller::Singleton<Controller::LuaState>::GetInstance().RunScript("print('Hello from lua!')");
	}
}

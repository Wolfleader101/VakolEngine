#include <iostream>
#include <Controller/Singleton.hpp>
#include <Controller/LuaState.hpp>

namespace Vakol {

	void Main()
	{
		std::cout << "Hello from core" << std::endl;

		Controller::Singleton<Controller::LuaState>::GetInstance().RunScript("print('Hello from lua!')");
	}
}

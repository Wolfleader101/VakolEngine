#include <iostream>
#include <sol/sol.hpp>

namespace Vakol {

	void Main()
	{
		std::cout << "Hello from core" << std::endl;

		sol::state lua;
		lua.open_libraries(sol::lib::base);

		lua.script("print('Hello from lua!')");
	}
}

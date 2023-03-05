#pragma once

#include <entt/entt.hpp>


namespace Vakol
{
	namespace ECS
	{
		namespace System
		{
			using reg = entt::registry;

			// -- READ --
			// if you add a function use the convention Type_Action()
			// will make it more intuitive to find names of funcs

		

			void Model_Draw(reg& registry);

			void Script_Init(reg& registry);
			void Script_Update(reg& registry);

		}
	}
}
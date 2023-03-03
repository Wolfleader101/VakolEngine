#include "System.hpp"

#include "Components.hpp"

namespace Vakol
{
	namespace System
	{
		void Vakol::System::Model_Draw(reg& registry)
		{
			registry.view<Components::Transform, Components::ModelType>
		}

		void Vakol::System::Script_Init(reg& registry)
		{
		}

		void Vakol::System::Script_Update(reg& registry)
		{
		}

	}
}
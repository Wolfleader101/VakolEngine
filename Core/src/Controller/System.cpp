#include "System.hpp"

#include <Model/Components.hpp>


namespace Vakol::Model::ECS::System
{
	void Model_Draw(reg& registry)
	{
		//registry.view<Components::Transform, Components::ModelType>().each(
		//	[&](auto& trans, auto& model) {
		//		/* draw model */
		//				
		//	}
		//);
	}

	void Script_Init(reg& registry)
	{
	}

	void Script_Update(reg& registry)
	{
	}

}

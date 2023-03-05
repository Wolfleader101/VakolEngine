#include <ECS/System.hpp>

#include <ECS/Components.hpp>

namespace Vakol
{
	namespace ECS
	{
		namespace System
		{
			void Model_Draw(reg& registry)
			{
				registry.view<Components::Transform, Components::ModelType>().each(
					[&](auto& trans, auto& model) {
						/* draw model */
						
					}
				);
			}

			void Script_Init(reg& registry)
			{
			}

			void Script_Update(reg& registry)
			{
			}

		}
	}
}
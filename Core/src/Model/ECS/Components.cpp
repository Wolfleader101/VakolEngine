#include <ECS/Components.hpp>

#include <Controller/Logger.hpp>

#include <istream>
#include <sstream>
#include <fstream>
#include <iostream>

#include <glad/glad.h>

namespace Vakol
{
	namespace ECS
	{
		namespace Components
		{
			Transform::Transform(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scale)
				: pos(pos), rot(rot), scale(scale) {};

			void Collider::GetBounds(const Mesh& msh, Bounds& bounds)
			{
				glm::vec3 centre = glm::vec3(0, 0, 0);

				glm::vec3 max = glm::vec3(std::numeric_limits<float>::min());
				glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());

				for (int i = 0; i < msh.positions.size(); i += 3)
				{
					centre.x += msh.positions[i];
					centre.y += msh.positions[i + 1];
					centre.z += msh.positions[i + 2];

					if (msh.positions[i] > max.x)
						max.x = msh.positions[i];
					if (msh.positions[i + 1] > max.y)
						max.y = msh.positions[i + 1];
					if (msh.positions[i + 2] > max.z)
						max.z = msh.positions[i + 2];

					if (msh.positions[i] < min.x)
						min.x = msh.positions[i];
					if (msh.positions[i + 1] < min.y)
						min.y = msh.positions[i + 1];
					if (msh.positions[i + 2] < min.z)
						min.z = msh.positions[i + 2];
				}

				centre /= msh.positions.size();

				bounds.min = rp3d::Vector3(min.x, min.y, min.z);
				bounds.max = rp3d::Vector3(max.x, max.y, max.z);
				bounds.center = rp3d::Vector3(centre.x, centre.y, centre.z);
				bounds.extents = rp3d::Vector3((max.x - min.x) / 2, (max.y - min.y) / 2, (max.z - min.z) / 2);
				bounds.size = rp3d::Vector3(max.x - min.x, max.y - min.y, max.z - min.z);

				float radius = 0.0f;

				for (int i = 0; i < msh.positions.size(); i += 3)
				{
					float dist = glm::distance(glm::vec3(msh.positions[i], msh.positions[i + 1], msh.positions[i + 2]), glm::vec3(centre.x, centre.y, centre.z));

					if (dist > radius)
						radius = dist;
				}

				bounds.radius = radius;
			}
		}
	}
}

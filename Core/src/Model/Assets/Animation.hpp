#pragma once

#include <vector>
#include <string>

#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#pragma warning(pop)

namespace Vakol::Model::Assets
{
	struct Transform
	{
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;
	};

	struct Animation
	{
		struct Channel
		{
			std::string name;
			std::vector<Transform> frames;
		};

		std::string name;
		float duration = 0.0f;
		float frame_rate = 0.0f;
		std::vector<Channel> channels;
	};
}
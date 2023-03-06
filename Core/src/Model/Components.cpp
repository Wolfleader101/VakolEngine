#include "Components.hpp"

#include <Controller/Logger.hpp>

#include <istream>
#include <sstream>
#include <fstream>
#include <iostream>

#include <glad/glad.h>


namespace Vakol::Model::ECS::Components
{
	Transform::Transform(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scale)
		: pos(pos), rot(rot), scale(scale) {};
}


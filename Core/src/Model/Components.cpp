#include "Components.hpp"

#include <glad/glad.h>

#include <Controller/Logger.hpp>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>

namespace Vakol::Model::Components {
    Transform::Transform(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scale)
        : pos(pos), rot(rot), scale(scale){};

    Script::Script(const std::string& scriptName) : script_name(scriptName){};
}  // namespace Vakol::Model::Components

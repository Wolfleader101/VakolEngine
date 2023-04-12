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

    Script::Script(const std::string& name) : script_name(name) {}

    Script::Script(const std::string& scriptName, Controller::LuaState& lua, Model::Entity& entity)
        : script_name(scriptName) {
        lua.RunFile("scripts/" + scriptName);

        sol::function init = lua.GetState()["init"];

        init(entity);
    };
    TagType::TagType(uint8_t type) : type(EntityType(type)) {}
    Tag::Tag(const std::string& tag) : tag(tag) {}
}  // namespace Vakol::Model::Components

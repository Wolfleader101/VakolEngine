#include "Components.hpp"

#include <glad/glad.h>

#include <Controller/AssetLoader/AssetLoader.hpp>

#include <Controller/Logger.hpp>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>

namespace Vakol::Model::Components {
    Transform::Transform(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scale)
        : pos(pos), rot(rot), scale(scale){};

    Script::Script(const std::string& name)
     : script_name(name){}

    Script::Script(const std::string& scriptName, Controller::LuaState& lua) : script_name(scriptName) {
        lua.RunFile("scripts/" + scriptName);

        sol::function init = lua.GetState()["init"];

        init();
    };

    Drawable::Drawable(std::string&& file) : name(std::move(file)) //WOW! EFFICIENT!
    {
        ModelPtr = Controller::AssetLoader::GetModel(name);
    }

    TagType::TagType(uint8_t type) : type(EntityType(type)) {};

    Tag::Tag(const std::string& tag) : tag(tag) {};

    PhysicsObject::PhysicsObject(std::shared_ptr<ScenePhysics> SP, unsigned int BodyType, unsigned int ShapeType)
    {
        owningWorld = SP;
        Type = (rp3d::BodyType) BodyType;
        ShapeName = (rp3d::CollisionShapeName)ShapeType;
    }
    
}  // namespace Vakol::Model::Components

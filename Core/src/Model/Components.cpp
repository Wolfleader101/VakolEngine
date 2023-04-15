#include "Components.hpp"

#include <glad/glad.h>

#include <Controller/AssetLoader/AssetLoader.hpp>

#include <Controller/Logger.hpp>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>

namespace Vakol::Model::Components 
{

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

    /*PhysicsObject::PhysicsObject(std::shared_ptr<ScenePhysics>& SP, unsigned int BodyType, unsigned int ShapeType, bool colliderOnly) 
        : owningWorld(SP), Type((rp3d::BodyType)BodyType), ShapeName((rp3d::CollisionShapeName)ShapeType), ColliderOnly(colliderOnly)
    {
        owningWorld = SP;
        Type = (rp3d::BodyType)BodyType;
        ShapeName = (rp3d::CollisionShapeName)ShapeType;
        ColliderOnly = colliderOnly;
    }*/

    Collider::Bounds getBounds(const Drawable& model)
    {

        Collider::Bounds bounds;

        rp3d::Vector3& centre = bounds.center;

        rp3d::Vector3& max = bounds.max;
        rp3d::Vector3& min = bounds.min;

        auto& firstVert = model.ModelPtr->meshes.at(0).vertices.at(0);

        max = min = rp3d::Vector3(firstVert.position.x, firstVert.position.y, firstVert.position.z);

        rp3d::Vector3 tempVert;

        for (auto& msh : model.ModelPtr->meshes)
        {
            for (auto& vertice : msh.vertices)
            {
                tempVert.x = vertice.position.x;
                tempVert.y = vertice.position.y;
                tempVert.z = vertice.position.z;

                max = rp3d::Vector3::max(max, tempVert);
                min = rp3d::Vector3::min(min, tempVert);

            }
        }

        bounds.center = (bounds.max + bounds.min) / 2.0f;
        bounds.extents = (bounds.max - bounds.min) / 2.0f;
        bounds.size = bounds.extents * 2;
        bounds.radius = bounds.extents.length();

        return bounds;

    }

    
    Collider::Collider(RigidBody& owner)
    {
        OwningBody = &owner;
    }

}  // namespace Vakol::Model::Components

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
    Transform::Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale)
        : pos(pos), rot(rot), scale(scale){};

    Script::Script(const std::string& name) : script_name(name) {}

    Script::Script(const std::string& scriptName, Controller::LuaState& lua, Model::Entity& entity,
                   Controller::Scene& scene)
        : script_name(scriptName) {
        lua.RunFile("scripts/" + scriptName);

        sol::function init = lua.GetState()["init"];

        init(scene, entity);
    };

    Drawable::Drawable(std::string&& file)
        : name(std::move(file))  // WOW! EFFICIENT!
    {
        model_ptr = Controller::AssetLoader::GetModel(name);
    }

    TagType::TagType(uint8_t type) : type(EntityType(type)){};

    Tag::Tag(const std::string& tag) : tag(tag){};

    Collider::Collider(RigidBody& owner, std::optional<Bounds> Data) {
        OwningBody = &owner;

        if (!Data.has_value()) return;

        bounds = Data.value();
    }

    RigidBody::RigidBody(std::shared_ptr<ScenePhysics> SP, std::optional<RigidData> DataR) {
        owningWorld = SP;

        if (!DataR.has_value()) return;

        Data = DataR.value();
    }

    Collider::Bounds getBounds(const Drawable& model) 
    {
        Collider::Bounds bounds;

        rp3d::Vector3& max = bounds.max;
        rp3d::Vector3& min = bounds.min;

        auto firstVert = *model.model_ptr->GetMeshes().begin()->GetVertexArray()->GetVertices().begin();

        max = min = rp3d::Vector3(firstVert->position.x, firstVert->position.y, firstVert->position.z);

        rp3d::Vector3 tempVert;

        for (auto& msh : model.model_ptr->GetMeshes()) 
        {
            for (auto& vertex : msh.GetVertexArray()->GetVertices()) 
            {
                tempVert.x = vertex.position.x;
                tempVert.y = vertex.position.y;
                tempVert.z = vertex.position.z;

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
}

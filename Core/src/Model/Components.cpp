#include "Components.hpp"

#include <glad/glad.h>

#include <Controller/AssetLoader/AssetLoader.hpp>
#include <Controller/Logger.hpp>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>

namespace Vakol::Model::Components {
    Transform::Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale)
        : pos(pos), rot(rot), scale(scale){};

    Script::Script(const std::string& name) : script_name(name) {}

    Script::Script(const std::string& scriptName, Controller::LuaState& lua, Model::Entity& entity,
                   Controller::Scene& scene)
        : script_name(scriptName) {
        lua.GetState()["scene"] = scene;
        lua.GetState()["entity"] = entity;

        lua.RunFile("scripts/" + scriptName);

        sol::function init = lua.GetState()["init"];
        init();
    };

    Drawable::Drawable(std::string&& file)
        : name(std::move(file))  // WOW! EFFICIENT!
    {
        model_ptr = Controller::AssetLoader::GetModel(name);
    }

    TagType::TagType(uint8_t type) : type(EntityType(type)){};

    Tag::Tag(const std::string& tag) : tag(tag){};

    void RigidBody::SetRigidData(const RigidData& data) {
        Data = data;

        RigidBodyPtr->setMass(Data.mass);
        RigidBodyPtr->setType((rp3d::BodyType)Type);
        RigidBodyPtr->enableGravity(Data.grav);
        RigidBodyPtr->setAngularDamping(Data.ADamp);
        RigidBodyPtr->setLinearDamping(Data.LDamp);
        RigidBodyPtr->setAngularLockAxisFactor(Data.AngularLock);
    }

    void RigidBody::ToggleGravity() {
        Data.grav = !Data.grav;
        RigidBodyPtr->enableGravity(Data.grav);
    }

    void RigidBody::SetBodyType(BodyType t) {
        Type = t;
        RigidBodyPtr->setType((rp3d::BodyType)Type);
    }

    void RigidBody::SetVelocity(const glm::vec3& vel) {
        RigidBodyPtr->setLinearVelocity(rp3d::Vector3(vel.x, vel.y, vel.z));
    }

    void RigidBody::SetAngularVelocity(const glm::vec3& vel) {
        RigidBodyPtr->setAngularVelocity(rp3d::Vector3(vel.x, vel.y, vel.z));
    }

    void RigidBody::SetAngularDamp(float Damp) { RigidBodyPtr->setAngularDamping(Damp); }

    void RigidBody::SetLinearDamp(float Damp) { RigidBodyPtr->setLinearDamping(Damp); }

    Collider::Collider(RigidBody& owner, std::optional<Bounds> Data) {
        OwningBody = &owner;

        if (!Data.has_value()) return;

        bounds = Data.value();
    }

    void Collider::SetBounds(const Bounds& data) { bounds = data; }

    Collider::Bounds getBounds(const Drawable& model) {
        Collider::Bounds bounds;

        rp3d::Vector3& max = bounds.max;
        rp3d::Vector3& min = bounds.min;

        // Assuming each vertex is represented by 3 floats (x, y, z).
        const std::vector<float>& vertices = model.model_ptr->GetMeshes().begin()->vertices();

        if (vertices.size() < 3) {
            throw std::runtime_error("Insufficient vertices data");
        }

        max = min = rp3d::Vector3(vertices[0], vertices[1], vertices[2]);

        rp3d::Vector3 tempVert;

        for (auto& msh : model.model_ptr->GetMeshes()) {
            const std::vector<float>& vertices = msh.vertices();

            for (size_t i = 0; i < vertices.size(); i += 3) {
                tempVert.x = vertices[i];
                tempVert.y = vertices[i + 1];
                tempVert.z = vertices[i + 2];

                max = rp3d::Vector3::max(max, tempVert);
                min = rp3d::Vector3::min(min, tempVert);
            }
        }

        bounds.center = (max + min) / 2.0f;
        bounds.extents = (max - min) / 2.0f;
        bounds.size = bounds.extents * 2;
        bounds.radius = bounds.extents.length() / 2.0f;

        return bounds;
    }
}  // namespace Vakol::Model::Components

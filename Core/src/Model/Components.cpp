#include "Components.hpp"

#include <glad/glad.h>

#include <Controller/AssetLoader/AssetLoader.hpp>
#include <Controller/Logger.hpp>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>

namespace Vakol::Model::Components {
    rp3d::Vector3 to_rp3d(const glm::vec3& v) { return {v.x, v.y, v.z}; }

    Transform::Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale)
        : pos(pos), rot(rot), scale(scale){};

    Script::Script(const std::string& name) : script_name(name) {}

    Script::Script(const std::string& scriptName, Controller::LuaState& lua, Model::Entity& entity,
                   Controller::Scene& scene)
        : script_name(scriptName) {
        lua.GetState()["scene"] = std::ref(scene);
        lua.GetState()["entity"] = entity;

        state = lua.GetState().create_table();
        lua.GetState()["state"] = state;

        lua.RunFile("scripts/" + scriptName);

        lua.RunFunction("init");
    };

    FSM::FSM(Controller::LuaState& lua) : lua(lua) {
        // Create a new table in the Lua state for the states
        states = lua.GetState().create_table();
    }

    void FSM::AddState(const std::string& stateName, sol::function callback) {
        // Add a new state to the states table
        states[stateName] = callback;
    }

    void FSM::ChangeState(const std::string& stateName) {
        // Change the current state
        currentState = stateName;
    }

    std::string FSM::GetState() {
        // Get the current state
        return currentState;
    }

    void FSM::Update() {
        // Call the callback for the current state
        sol::function callback = states[currentState];
        lua.RunFunction(callback);
    }

    Drawable::Drawable(std::string&& file)
        : name(std::move(file))  // WOW! EFFICIENT!
    {
        model_ptr = Controller::AssetLoader::GetModel(name, 1.0f, false);
    }

    TagType::TagType(uint8_t type) : type(ENTITY_TYPE(type)){};

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

    void RigidBody::SetBodyType(const BODY_TYPE t) {
        Type = t;

        RigidBodyPtr->setType(static_cast<rp3d::BodyType>(Type));
    }

    void RigidBody::SetVelocity(const glm::vec3& vel) const {
        RigidBodyPtr->setLinearVelocity(rp3d::Vector3(vel.x, vel.y, vel.z));
    }

    void RigidBody::SetAngularVelocity(const glm::vec3& vel) const {
        RigidBodyPtr->setAngularVelocity(rp3d::Vector3(vel.x, vel.y, vel.z));
    }

    void RigidBody::SetAngularDamp(const float damp) const { RigidBodyPtr->setAngularDamping(damp); }

    void RigidBody::SetLinearDamp(const float damp) const { RigidBodyPtr->setLinearDamping(damp); }

    Collider::Collider(RigidBody& owner, const std::optional<Bounds>& data) {
        OwningBody = &owner;

        if (!data.has_value()) return;

        bounds = data.value();
    }

    void Collider::SetBounds(const Bounds& data) { bounds = data; }

    // THIS HAS BEEN MODIFIED BY ME (CALEB)
    Collider::Bounds GetBounds(const Drawable& model) {
        Collider::Bounds bounds;

        rp3d::Vector3& max = bounds.max;
        rp3d::Vector3& min = bounds.min;

        // Assuming each vertex is represented by 3 floats (x, y, z).
        std::vector<Vertex> vertices = model.model_ptr->meshes().begin()->c_vertices();

        if (vertices.size() < 3) {
            throw std::runtime_error("Insufficient vertices data");
        }

        const auto& position = vertices.begin()->position;
        max = min = rp3d::Vector3(position.x, position.y, position.z);

        for (auto& msh : model.model_ptr->meshes()) {
            vertices = msh.c_vertices();

            for (const auto& vertex : vertices) {
                const auto temp = to_rp3d(vertex.position);

                max = rp3d::Vector3::max(max, temp);
                min = rp3d::Vector3::min(min, temp);
            }
        }

        bounds.center = (max + min) / 2.0f;
        bounds.extents = (max - min) / 2.0f;
        bounds.size = bounds.extents * 2;
        bounds.radius = bounds.extents.length() / 2.0f;

        return bounds;
    }
}  // namespace Vakol::Model::Components

#include "Components.hpp"

#include <Controller/AssetLoader/AssetLoader.hpp>
#include <Controller/Scene.hpp>

namespace Vakol::Model::Components {
    rp3d::Vector3 to_rp3d(const glm::vec3& v) { return {v.x, v.y, v.z}; }

    Transform::Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale)
        : pos(pos), rot(rot), scale(scale){};

    Script::Script(std::string& name) : script_name(std::move(name)) {}

    Script::Script(const std::string& script, Controller::LuaState& lua, Entity& entity, Controller::Scene& scene) : script_name(script)
	{
        lua.GetState()["scene"] = std::ref(scene);
        lua.GetState()["entity"] = entity;

        state = lua.GetState().create_table();
        lua.GetState()["state"] = state;

        lua.RunFile("scripts/" + script);

        lua.RunFunction("init");
    };

    FSM::FSM(Controller::LuaState& lua) : lua(lua) {
        // Create a new table in the Lua state for the states
        states = lua.GetState().create_table();
    }

    void FSM::AddState(const std::string& stateName, const sol::function& callback) {
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
        const sol::function callback = states[currentState];
        lua.RunFunction(callback);
    }

    Drawable::Drawable(std::string&& file)
        : name(std::move(file))  // WOW! EFFICIENT!
    {
        model_ptr = Controller::AssetLoader::GetModel(name, 1.0f, false, true).first;
    }

    TagType::TagType(uint8_t type) : type(static_cast<ENTITY_TYPE>(type)){};

    Tag::Tag(std::string& tag) : tag(std::move(tag)){};

    void RigidBody::SetRigidData(const RigidData& data) {
        Data = data;

        RigidBodyPtr->setMass(Data.mass);
        RigidBodyPtr->setType(static_cast<rp3d::BodyType>(Type));
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
   glm::mat4 to_rp3d_mat4(const Transform& transform) {
        glm::mat4 mat(1.0f);
        mat = glm::translate(mat, transform.pos);
        mat = glm::scale(mat, transform.scale);
        mat = glm::rotate(mat, transform.rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
        mat = glm::rotate(mat, transform.rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
        mat = glm::rotate(mat, transform.rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
        return mat;
    }

    rp3d::Vector3 transformVertex(const glm::mat4& matrix, const rp3d::Vector3& vertex) {
        glm::vec4 glmVertex(vertex.x, vertex.y, vertex.z, 1.0f);
        glmVertex = matrix * glmVertex;
        return rp3d::Vector3(glmVertex.x, glmVertex.y, glmVertex.z);
    }

    Collider::Bounds GetBounds(const Drawable& model, const Transform& transform) {
        Collider::Bounds bounds;

        rp3d::Vector3& max = bounds.max;
        rp3d::Vector3& min = bounds.min;

        glm::mat4 transformMat = to_rp3d_mat4(transform);

        auto& vertices = model.model_ptr->meshes().begin()->vertices();

        VK_ASSERT(vertices.size() >= 3, "\n\nInsufficient vertices data");

        const auto& position = vertices.begin()->position;
        rp3d::Vector3 transformedPosition = transformVertex(transformMat, rp3d::Vector3(position.x, position.y, position.z));
        max = min = transformedPosition;

        for (const auto& msh : model.model_ptr->c_meshes()) {
            vertices = msh.c_vertices();

            for (const auto& vertex : vertices) {
                const auto temp = transformVertex(transformMat, to_rp3d(vertex.position));
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

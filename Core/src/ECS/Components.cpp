#include "ECS/Components.hpp"

#include "SceneManager/Scene.hpp"

namespace Vakol::Components
{
    rp3d::Vector3 to_rp3d(const Math::Vec3& v)
    {
        return {v.x, v.y, v.z};
    }

    rp3d::Quaternion to_rp3d(const Math::Quat& q)
    {
        return {q.x, q.y, q.z, q.w};
    }

    Transform::Transform(const Math::Vec3& pos, const Math::Quat& rot, const Math::Vec3& scale)
        : pos(pos), rot(rot), scale(scale)
    {
    }

    FSM::FSM(LuaTable table) : states(table)
    {
    }

    void FSM::AddState(const std::string& stateName, LuaFunction& callback)
    {
        // Add a new state to the states table
        states[stateName] = callback;
    }

    void FSM::ChangeState(const std::string& stateName)
    {
        // Change the current state
        currentState = stateName;
    }

    std::string FSM::GetState()
    {
        // Get the current state
        return currentState;
    }

    void FSM::Update()
    {
        // Call the callback for the current state
        const LuaFunction callback = states[currentState];
        callback();
    }

    TagType::TagType(uint8_t type) : type(static_cast<ENTITY_TYPE>(type)){};

    Tag::Tag(std::string& tag) : tag(std::move(tag)){};

    void RigidBody::SetRigidData(const RigidData& data)
    {
        Data = data;

        RigidBodyPtr->setMass(Data.mass);
        RigidBodyPtr->setType(static_cast<rp3d::BodyType>(Type));
        RigidBodyPtr->enableGravity(Data.grav);
        RigidBodyPtr->setAngularDamping(Data.ADamp);
        RigidBodyPtr->setLinearDamping(Data.LDamp);
        RigidBodyPtr->setAngularLockAxisFactor(Data.AngularLock);
    }

    void RigidBody::ToggleGravity()
    {
        Data.grav = !Data.grav;
        RigidBodyPtr->enableGravity(Data.grav);
    }

    void RigidBody::SetBodyType(const BODY_TYPE t)
    {
        Type = t;

        RigidBodyPtr->setType(static_cast<rp3d::BodyType>(Type));
    }

    void RigidBody::SetVelocity(const Math::Vec3& vel) const
    {
        RigidBodyPtr->setLinearVelocity(rp3d::Vector3(vel.x, vel.y, vel.z));
    }

    void RigidBody::SetAngularVelocity(const Math::Vec3& vel) const
    {
        RigidBodyPtr->setAngularVelocity(rp3d::Vector3(vel.x, vel.y, vel.z));
    }

    void RigidBody::ApplyForce(const Math::Vec3& force) const
    {
        RigidBodyPtr->applyWorldForceAtCenterOfMass(rp3d::Vector3(force.x, force.y, force.z));
    }

    void RigidBody::SetAngularDamp(const float damp) const
    {
        RigidBodyPtr->setAngularDamping(damp);
    }

    void RigidBody::SetLinearDamp(const float damp) const
    {
        RigidBodyPtr->setLinearDamping(damp);
    }

    Collider::Collider(RigidBody& owner, const std::optional<Bounds>& data)
    {
        OwningBody = &owner;

        if (!data.has_value())
            return;

        bounds = data.value();
    }

    void Collider::SetBounds(const Bounds& data)
    {
        bounds = data;
    }

    // THIS HAS BEEN MODIFIED BY ME (CALEB)
    Math::Mat4 to_rp3d_mat4(const Transform& transform)
    {
        Math::Mat4 mat(1.0f);
        mat = Math::Translate(mat, transform.pos);
        mat = Math::Scale(mat, transform.scale);
        mat = Math::Rotate(mat, transform.rot.x, Math::Vec3(1.0f, 0.0f, 0.0f));
        mat = Math::Rotate(mat, transform.rot.y, Math::Vec3(0.0f, 1.0f, 0.0f));
        mat = Math::Rotate(mat, transform.rot.z, Math::Vec3(0.0f, 0.0f, 1.0f));
        return mat;
    }

    rp3d::Vector3 transformVertex(const Math::Mat4& matrix, const rp3d::Vector3& vertex)
    {
        Math::Vec4 glmVertex(vertex.x, vertex.y, vertex.z, 1.0f);
        glmVertex = matrix * glmVertex;

        return {glmVertex.x, glmVertex.y, glmVertex.z};
    }

    //Collider::Bounds GetBounds(const Rendering::Drawable& model, const Transform& transform)
    //{
    //    //Collider::Bounds bounds;

    //    //rp3d::Vector3& max = bounds.max;
    //    //rp3d::Vector3& min = bounds.min;

    //    //const Math::Mat4 transformMat = to_rp3d_mat4(transform);

    //    //auto& vertices = model.meshes().begin()->vertices();

    //    //if (vertices.size() < 3)
    //    //{
    //    //    VK_CRITICAL("Collider::Bounds::GetBounds() - Insufficient vertices data");
    //    //    return bounds;
    //    //}

    //    //const auto& position = vertices.begin()->position;

    //    //const rp3d::Vector3 transformedPosition =
    //    //    transformVertex(transformMat, rp3d::Vector3(position.x, position.y, position.z));

    //    //max = min = transformedPosition;

    //    //for (const auto& msh : model.model_ptr->c_meshes())
    //    //{
    //    //    vertices = msh.c_vertices();

    //    //    for (const auto& vertex : vertices)
    //    //    {
    //    //        const auto temp = transformVertex(transformMat, to_rp3d(vertex.position));
    //    //        max = rp3d::Vector3::max(max, temp);
    //    //        min = rp3d::Vector3::min(min, temp);
    //    //    }
    //    //}

    //    //bounds.center = (max + min) / 2.0f;
    //    //bounds.extents = (max - min) / 2.0f;
    //    //bounds.size = bounds.extents * 2;
    //    //bounds.radius = bounds.extents.length() / 2.0f;

    //    //return bounds;
    //}

    void GUID::GenNewGUID()
    {
        id = xg::newGuid();
    }

    bool GUID::operator==(const GUID& other) const
    {
        return id == other.id;
    }

    bool GUID::operator!=(const GUID& other) const
    {
        return id != other.id;
    }

    bool GUID::operator<(const GUID& other) const
    {
        return id < other.id;
    }

} // namespace Vakol::Components

#include "LuaAccess.hpp"

#include "Physics/PhysicsEngine.hpp"
#include "Physics/PhysicsScene.hpp"

namespace Vakol
{
    void RegisterPhysics(sol::state& lua)
    {
        auto rbT = lua.new_usertype<RigidBody>("RigidBody");

        lua.new_enum("BodyType", "Static", BodyType::Static, "Kinematic", BodyType::Kinematic, "Dynamic",
                     BodyType::Dynamic);

        rbT.set("type", &RigidBody::type);
        rbT.set("mass", &RigidBody::mass);
        rbT.set("hasGravity", &RigidBody::hasGravity);
        rbT.set("centerOfMass", &RigidBody::centerOfMass);
        rbT.set("force", &RigidBody::force);
        rbT.set("torque", &RigidBody::torque);
        rbT.set("linearVelocity", &RigidBody::linearVelocity);
        rbT.set("angularVelocity", &RigidBody::angularVelocity);

        // auto physT = lua.new_usertype<PhysicsEngine>("PhysicsEngine");

        // AABBCollider CreateAABBCollider(Math::Vec3& halfExtents);

        // SphereCollider CreateSphereCollider(double radius);

        // CapsuleCollider CreateCapsuleCollider(double radius, double height);

        // MeshCollider CreateMeshCollider(std::vector<Math::Point>& vertices, std::vector<unsigned int>& indices);

        // void AttachCollider(RigidBody& rb, AABBCollider& collider);

        // void AttachCollider(RigidBody& rb, SphereCollider& collider);

        // void AttachCollider(RigidBody& rb, CapsuleCollider& collider);

        // void AttachCollider(RigidBody& rb, MeshCollider& collider);

        // physT.set_function("create_box_collider")
    }

} // namespace Vakol
#include "LuaAccess.hpp"

#include "Physics/PhysicsEngine.hpp"
#include "Physics/PhysicsScene.hpp"

#include "Logger/Logger.hpp"
namespace Vakol
{
    void RegisterPhysics(sol::state& lua)
    {
        auto rbT = lua.new_usertype<RigidBody>("RigidBody");

        lua.new_enum("BodyType", "Static", BodyType::Static, "Kinematic", BodyType::Kinematic, "Dynamic",
                     BodyType::Dynamic);

        lua["BodyType"] = lua.create_table_with("Static", BodyType::Static, "Kinematic", BodyType::Kinematic, "Dynamic",
                                                BodyType::Dynamic);

        rbT.set("type", &RigidBody::type);
        rbT.set("mass", &RigidBody::mass);
        rbT.set("bounciness", &RigidBody::bounciness);
        rbT.set("hasGravity", &RigidBody::hasGravity);
        rbT.set("centerOfMass", &RigidBody::centerOfMass);
        rbT.set("force", &RigidBody::force);
        rbT.set("torque", &RigidBody::torque);
        rbT.set("linearVelocity", &RigidBody::linearVelocity);
        rbT.set("angularVelocity", &RigidBody::angularVelocity);
        rbT.set("pos_lock", &RigidBody::lockPosition);
        rbT.set("rot_lock", &RigidBody::lockRotation);
        rbT.set("is_colliding", sol::property([](RigidBody& rb) { return rb.collisionData->isColliding; }));

        rbT.set_function("add_force", [](RigidBody& rb, Math::Vec3& force) {
            rb.force += force;
            rb.isSleeping = false;
            rb.sleepCounter = 0;
        });

        rbT.set_function("add_torque", [](RigidBody& rb, Math::Vec3& torque) {
            rb.torque += torque;
            rb.isSleeping = false;
            rb.sleepCounter = 0;
        });

        rbT.set_function("apply_impulse", [](RigidBody& rb, Math::Vec3& impulse) {
            rb.linearVelocity += impulse;
            rb.isSleeping = false;
            rb.sleepCounter = 0;
        });

        rbT.set_function("apply_angular_impulse", [](RigidBody& rb, Math::Vec3& impulse, Math::Vec3& point) {
            Math::Vec3 torque = Math::Cross(point, impulse);
            rb.angularVelocity += torque * rb.invInertiaTensor;
            rb.isSleeping = false;
            rb.sleepCounter = 0;
        });

        auto rct = lua.new_usertype<RayCastHitInfo>("RayCastHitInfo");

        rct.set("point", &RayCastHitInfo::point);
        rct.set("normal", &RayCastHitInfo::normal);
        rct.set("distance", &RayCastHitInfo::distance);
        rct.set("hit", &RayCastHitInfo::hit);
    }

} // namespace Vakol
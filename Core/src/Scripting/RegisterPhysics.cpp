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

        lua["BodyType"] = lua.create_table_with("Static", BodyType::Static, "Kinematic", BodyType::Kinematic, "Dynamic",
                                                BodyType::Dynamic);

        rbT.set("type", &RigidBody::type);
        rbT.set("mass", &RigidBody::mass);
        //rbT.set("bounciness", &RigidBody::bounciness);
        rbT.set("useGravity", &RigidBody::useGravity);
        //rbT.set("centerOfMass", &RigidBody::centerOfMass);
        //rbT.set("force", &RigidBody::force);
        //rbT.set("torque", &RigidBody::torque);
        rbT.set("linearVelocity", &RigidBody::linearVelocity);
        rbT.set("angularVelocity", &RigidBody::angularVelocity);
        rbT.set("is_colliding", sol::property([](RigidBody& rb) { return rb.collisionData->isColliding; }));

        //rbT.set_function("add_force", [](RigidBody& rb, Math::Vec3& force) { rb.force += force; });

        auto rct = lua.new_usertype<RayCastHitInfo>("RayCastHitInfo");

        rct.set("point", &RayCastHitInfo::point);
        rct.set("normal", &RayCastHitInfo::normal);
        rct.set("distance", &RayCastHitInfo::distance);
        rct.set("hit", &RayCastHitInfo::hit);
    }

} // namespace Vakol
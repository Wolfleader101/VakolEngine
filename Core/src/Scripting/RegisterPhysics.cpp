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
        rbT.set("hasGravity", &RigidBody::hasGravity);
        rbT.set("centerOfMass", &RigidBody::centerOfMass);
        rbT.set("force", &RigidBody::force);
        rbT.set("torque", &RigidBody::torque);
        rbT.set("linearVelocity", &RigidBody::linearVelocity);
        rbT.set("angularVelocity", &RigidBody::angularVelocity);
    }

} // namespace Vakol
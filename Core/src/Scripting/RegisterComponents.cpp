#include "ECS/Components.hpp"
#include "LuaAccess.hpp"

namespace Vakol
{
    void RegisterTransform(sol::state& lua)
    {
        auto transform_type = lua.new_usertype<Components::Transform>("Transform");

        transform_type["pos"] = &Components::Transform::pos;
        transform_type["rot"] = &Components::Transform::eulerAngles;
        transform_type["scale"] = &Components::Transform::scale;
    }

    void RegisterTerrain(sol::state& lua)
    {
    }

    void RegisterFSM(sol::state& lua)
    {
        auto fsm_type = lua.new_usertype<Components::FSM>("FSM");
        fsm_type.set_function("get_state", &Components::FSM::GetState);
        fsm_type.set_function("change_state", &Components::FSM::ChangeState);
        fsm_type.set_function("add_state",
                              [](Components::FSM* self, std::string stateName, sol::protected_function callback) {
                                  self->AddState(stateName, callback);
                              });
        fsm_type.set_function("update", &Components::FSM::Update);
    }

    void RegisterRigidBody(sol::state& lua)
    {
        // auto rigidType = lua.new_usertype<Components::RigidBody>("RigidBody");

        // rigidType["use_transform"] = &Components::RigidBody::use_transform;
        // rigidType["is_colliding"] = &Components::RigidBody::is_colliding;

        // lua["BodyType"] = lua.create_table_with("Static", Components::RigidBody::BODY_TYPE::STATIC, "Kinematic",
        //                                         Components::RigidBody::BODY_TYPE::KINEMATIC, "Dynamic",
        //                                         Components::RigidBody::BODY_TYPE::DYNAMIC);

        // rigidType["BodyType"] = &Components::RigidBody::Type;

        // auto rigidDataType = lua.new_usertype<Components::RigidBody::RigidData>("RigidData");

        // rigidDataType["mass"] = &Components::RigidBody::RigidData::mass;
        // rigidDataType["gravity"] = &Components::RigidBody::RigidData::grav;
        // rigidDataType["linear_damp"] = &Components::RigidBody::RigidData::LDamp;
        // rigidDataType["angular_damp"] = &Components::RigidBody::RigidData::ADamp;

        // rigidType.set_function("set_data",
        //                        [](Components::RigidBody* rigid, const Components::RigidBody::RigidData& data) {
        //                            rigid->SetRigidData(data);
        //                        });

        // rigidType.set_function("toggle_gravity", [](Components::RigidBody* rigid) { rigid->ToggleGravity(); });

        // rigidType.set_function("set_data",
        //                        [](Components::RigidBody* rigid, const Components::RigidBody::RigidData& data) {
        //                            rigid->SetRigidData(data);
        //                        });

        // rigidType.set_function("toggle_gravity", [](Components::RigidBody* rigid) { rigid->ToggleGravity(); });

        // rigidType.set_function("set_body_type",
        //                        [](Components::RigidBody* rigid, const Components::RigidBody::BODY_TYPE type) {
        //                            rigid->SetBodyType(type);
        //                        });

        // rigidType.set_function(
        //     "set_velocity", [](const Components::RigidBody* rigid, const Math::Vec3& vel) { rigid->SetVelocity(vel);
        //     });

        // rigidType.set_function("set_angular_velocity", [](const Components::RigidBody* rigid, const Math::Vec3& vel)
        // {
        //     rigid->SetAngularVelocity(vel);
        // });

        // rigidType.set_function("set_linear_damp", [](const Components::RigidBody* rigid, const float damp) {
        //     rigid->SetLinearDamp(damp);
        // });

        // rigidType.set_function("set_angular_damp", [](const Components::RigidBody* rigid, const float damp) {
        //     rigid->SetAngularDamp(damp);
        // });

        // rigidType.set_function("apply_force",
        //                        [](Components::RigidBody* rigid, const Math::Vec3& force) { rigid->ApplyForce(force);
        //                        });
    }

    void RegisterCollider(sol::state& lua)
    {
        // auto colliderType = lua.new_usertype<Components::Collider>("Collider");

        // lua["Shape"] = lua.create_table_with(
        //     "Box", Components::Collider::ShapeName::BOX, "Sphere", Components::Collider::ShapeName::SPHERE,
        //     "Capsule", Components::Collider::ShapeName::CAPSULE, "TriangleMesh",
        //     Components::Collider::ShapeName::TRIANGLE_MESH);

        // colliderType["Shape"] = &Components::Collider::ShapeName;

        // auto ColliderBoundsType = lua.new_usertype<Components::Collider::Bounds>("ColliderBounds");

        // ColliderBoundsType["min"] = &Components::Collider::Bounds::min;
        // ColliderBoundsType["max"] = &Components::Collider::Bounds::max;
        // ColliderBoundsType["size"] = &Components::Collider::Bounds::size;
        // ColliderBoundsType["center"] = &Components::Collider::Bounds::center;
        // ColliderBoundsType["extents"] = &Components::Collider::Bounds::extents;
        // ColliderBoundsType["radius"] = &Components::Collider::Bounds::radius;

        // colliderType["bounds"] = &Components::Collider::bounds;

        // colliderType.set_function("set_bounds",
        //                           [](Components::Collider* collider, const Components::Collider::Bounds& bounds) {
        //                               collider->SetBounds(bounds);
        //                           });
    }
} // namespace Vakol
#include "ECS/Components.hpp"
#include "LuaAccess.hpp"

namespace Vakol
{
    void RegisterTransform(sol::state& lua)
    {
        auto transform_type = lua.new_usertype<Components::Transform>("Transform");

        transform_type["pos"] = &Components::Transform::pos;
        transform_type["rot"] = &Components::Transform::eulerAngles;

        // TODO look into why this doesnt seem to work, see ya in 2024 losers
        // transform_type.set("rot", sol::property(
        //                               [](const Components::Transform& transform) {
        //                                   return Math::RadToDeg(Math::EulerFromQuat(transform.rot));
        //                               },
        //                               [](Components::Transform& transform, const Math::Vec3& euler) {
        //                                   transform.rot = Math::Quat(Math::DegToRad(euler));
        //                               }));
        transform_type["scale"] = &Components::Transform::scale;
        transform_type.set("forward", sol::property([](const Components::Transform& transform) {
                               return transform.rot * Math::Vec3(0.0f, 0.0f, 1.0f);
                           }));
    }
} // namespace Vakol
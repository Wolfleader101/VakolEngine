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
        transform_type.set("forward", sol::property([](const Components::Transform& transform) {
                               return transform.rot * Math::Vec3(0.0f, 0.0f, -1.0f);
                           }));
    }
} // namespace Vakol
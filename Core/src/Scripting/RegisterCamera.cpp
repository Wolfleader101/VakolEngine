#include "Camera/Camera.hpp"
#include "LuaAccess.hpp"

namespace Vakol
{
    void RegisterCamera(sol::state& lua)
    {
        auto camera_type = lua.new_usertype<Camera>("Camera");

        camera_type.set_function("get_pos", &Camera::GetPos);
        camera_type.set_function("set_pos", &Camera::SetPos);
        camera_type.set_function("get_forward", &Camera::GetForward);
        camera_type.set_function("get_right", &Camera::GetRight);

        camera_type.set_function("get_pitch", &Camera::GetPitch);
        camera_type.set_function("set_pitch", &Camera::SetPitch);

        camera_type.set_function("get_yaw", &Camera::GetYaw);
        camera_type.set_function("set_yaw", &Camera::SetYaw);
    }

} // namespace Vakol
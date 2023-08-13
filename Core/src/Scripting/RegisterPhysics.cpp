#include "LuaAccess.hpp"

#include <reactphysics3d/reactphysics3d.h>

namespace Vakol
{
    // TODO remove this later, just used for rp3d vec
    void RegisterPhysics(sol::state& lua)
    {
        // TODO get rid of rp3d vec
        auto rp3dVec3 = lua.new_usertype<rp3d::Vector3>("PhyVec3"); // need for collider
        rp3dVec3["x"] = &rp3d::Vector3::x;
        rp3dVec3["y"] = &rp3d::Vector3::y;
        rp3dVec3["z"] = &rp3d::Vector3::z;
    }

} // namespace Vakol
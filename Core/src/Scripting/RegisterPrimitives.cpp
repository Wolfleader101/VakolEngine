#include "LuaAccess.hpp"

#include "Primitives/Primitives.hpp"

namespace Vakol
{
    void RegisterPrimitives(sol::state& lua)
    {
        // Define the ShapeType enum
        lua.new_enum("ShapeType", "SPHERE", Primitives::ShapeType::SPHERE, "CUBE", Primitives::ShapeType::CUBE);

        auto primitives_type = lua.new_usertype<Primitives>("Primitives"); // Create a new usertype in Lua

        // Register functions so they can be called from Lua
        primitives_type.set_function("create_sphere", &Primitives::CreateSphere);
        primitives_type.set_function("create_cube", &Primitives::CreateCube);
        primitives_type.set_function("get_model", &Primitives::GetModel);
        primitives_type.set_function("get_guid_by_name", &Primitives::GetGuidByName);
    }
} // namespace Vakol

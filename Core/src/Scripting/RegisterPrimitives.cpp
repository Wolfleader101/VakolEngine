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
        primitives_type.set_function(
            "create_cube",
            sol::overload([](Primitives& self, const std::string& name) { self.CreateCube(name); },
                          [](Primitives& self, const std::string& name, const Math::Vec3& halfExtents) {
                              self.CreateCube(name, halfExtents);
                          })); // Overload the create_cube function so it can be called with 1 or 2 arguments
        primitives_type.set_function("get_model", &Primitives::GetModel);
    }
} // namespace Vakol

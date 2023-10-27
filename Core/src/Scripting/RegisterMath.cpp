#include "LuaAccess.hpp"

#include "Math/Math.hpp"

std::vector<Vakol::Math::Mat4> create_mat4_vector(const size_t reserve)
{
    std::vector<Vakol::Math::Mat4> vector;

    vector.reserve(reserve);

    return vector;
}

namespace Vakol
{
    void RegisterMath(sol::state& lua)
    {

        lua.set_function("lerp",
                         [](const float a, const float b, const float t) -> float { return Math::Lerp(a, b, t); });

        lua.set_function("rad2deg", [](const Math::Vec3& v) -> Math::Vec3 { return Math::RadToDeg(v); });
        lua.set_function("normalize", [](const Math::Vec3& v) -> Math::Vec3 { return Math::Normalized(v); });
        lua.set_function("atan2", [](const float y, const float x) -> float { return std::atan2f(y, x); });

        lua.set_function("distance",
                         [](const Math::Vec3& u, const Math::Vec3& v) -> float { return Math::Distance(u, v); });

        lua.set_function("distance_sq",
                         [](const Math::Vec3& u, const Math::Vec3& v) -> float { return Math::DistanceSq(u, v); });

        {
            sol::constructors<Math::Vec2(), Math::Vec2(float), Math::Vec2(float, float)> ctor; // allow for constructors

            auto div_overload = sol::overload([](const Math::Vec2& v, const float k) -> Math::Vec2 { return v / k; },
                                              [](const float k, const Math::Vec2& v) -> Math::Vec2 { return k / v; });

            auto mul_overload =
                sol::overload([](const Math::Vec2& u, const Math::Vec2& v) -> Math::Vec2 { return u * v; },
                              [](const Math::Vec2& v, const float k) -> Math::Vec2 { return v * k; },
                              [](const float k, const Math::Vec2& v) -> Math::Vec2 { return k * v; });

            auto add_overload =
                sol::overload([](const Math::Vec2& u, const Math::Vec2& v) -> Math::Vec2 { return u + v; });

            auto sub_overload =
                sol::overload([](const Math::Vec2& u, const Math::Vec2& v) -> Math::Vec2 { return u - v; });

            auto vec2 =
                lua.new_usertype<Math::Vec2>("Vector2", ctor, sol::meta_function::multiplication, mul_overload,
                                             sol::meta_function::division, div_overload, sol::meta_function::addition,
                                             add_overload, sol::meta_function::subtraction, sub_overload);

            vec2.set_function("magnitude", [](const Math::Vec2& v) -> float { return Math::Magnitude(v); });

            vec2["x"] = &Math::Vec2::x;
            vec2["y"] = &Math::Vec2::y;

            vec2["r"] = &Math::Vec2::r;
            vec2["g"] = &Math::Vec2::g;
        }

        {
            sol::constructors<Math::Vec3(), Math::Vec3(float), Math::Vec3(float, float, float)>
                ctor; // allow for constructors

            auto div_overload = sol::overload([](const Math::Vec3& v, const float k) -> Math::Vec3 { return v / k; },
                                              [](const float k, const Math::Vec3& v) -> Math::Vec3 { return k / v; });

            auto mul_overload =
                sol::overload([](const Math::Vec3& u, const Math::Vec3& v) -> Math::Vec3 { return u * v; },
                              [](const Math::Vec3& v, const float k) -> Math::Vec3 { return v * k; },
                              [](const float k, const Math::Vec3& v) -> Math::Vec3 { return k * v; },
                              [](const Math::Vec3& v, const Math::Mat3& m) -> Math::Vec3 { return v * m; },
                              [](const Math::Mat3& m, const Math::Vec3& v) -> Math::Vec3 { return m * v; });

            auto add_overload =
                sol::overload([](const Math::Vec3& u, const Math::Vec3& v) -> Math::Vec3 { return u + v; });

            auto sub_overload =
                sol::overload([](const Math::Vec3& u, const Math::Vec3& v) -> Math::Vec3 { return u - v; });

            auto vec3 =
                lua.new_usertype<Math::Vec3>("Vector3", ctor, sol::meta_function::multiplication, mul_overload,
                                             sol::meta_function::division, div_overload, sol::meta_function::addition,
                                             add_overload, sol::meta_function::subtraction, sub_overload);

            vec3.set_function("magnitude", [](const Math::Vec3& v) -> float { return Math::Magnitude(v); });
            vec3.set_function("magnitude_sq", [](const Math::Vec3& v) -> float { return Math::MagnitudeSq(v); });

            vec3.set_function("length", [](const Math::Vec3& v) -> float { return Math::Magnitude(v); });
            vec3.set_function("length_sq", [](const Math::Vec3& v) -> float { return Math::MagnitudeSq(v); });

            vec3.set_function("normalize", [](const Math::Vec3& v) -> Math::Vec3 { return Math::Normalized(v); });

            vec3.set_function("negate", [](const Math::Vec3& v) -> Math::Vec3 { return -v; });

            vec3["x"] = &Math::Vec3::x;
            vec3["y"] = &Math::Vec3::y;
            vec3["z"] = &Math::Vec3::z;

            vec3["r"] = &Math::Vec3::r;
            vec3["g"] = &Math::Vec3::g;
            vec3["b"] = &Math::Vec3::b;
        }

        {
            sol::constructors<Math::BVec3(), Math::BVec3(bool), Math::BVec3(bool, bool, bool)>
                ctor; // allow for constructors
            auto vec3 = lua.new_usertype<Math::BVec3>("BVector3", ctor);

            vec3["x"] = &Math::BVec3::x;
            vec3["y"] = &Math::BVec3::y;
            vec3["z"] = &Math::BVec3::z;
        }

        {
            sol::constructors<Math::Vec4(), Math::Vec4(float), Math::Vec4(float, float, float, float)>
                ctor; // allow for constructors

            auto div_overload = sol::overload([](const Math::Vec4& v, const float k) -> Math::Vec4 { return v / k; },
                                              [](const float k, const Math::Vec4& v) -> Math::Vec4 { return k / v; });

            auto mul_overload =
                sol::overload([](const Math::Vec4& u, const Math::Vec4& v) -> Math::Vec4 { return u * v; },
                              [](const Math::Vec4& v, const float k) -> Math::Vec4 { return v * k; },
                              [](const float k, const Math::Vec4& v) -> Math::Vec4 { return k * v; },
                              [](const Math::Mat4& m, const Math::Vec4& v) -> Math::Vec4 { return m * v; },
                              [](const Math::Vec4& v, const Math::Mat4& m) -> Math::Vec4 { return v * m; });

            auto add_overload =
                sol::overload([](const Math::Vec4& u, const Math::Vec4& v) -> Math::Vec4 { return u + v; });

            auto sub_overload =
                sol::overload([](const Math::Vec4& u, const Math::Vec4& v) -> Math::Vec4 { return u - v; });

            auto vec4 =
                lua.new_usertype<Math::Vec4>("Vector4", ctor, sol::meta_function::multiplication, mul_overload,
                                             sol::meta_function::division, div_overload, sol::meta_function::addition,
                                             add_overload, sol::meta_function::subtraction, sub_overload);

            vec4["x"] = &Math::Vec4::x;
            vec4["y"] = &Math::Vec4::y;
            vec4["z"] = &Math::Vec4::z;
            vec4["w"] = &Math::Vec4::w;

            vec4["r"] = &Math::Vec4::r;
            vec4["g"] = &Math::Vec4::g;
            vec4["b"] = &Math::Vec4::b;
            vec4["a"] = &Math::Vec4::a;
        }

        {
            sol::constructors<Math::Mat4(), Math::Mat4(float),
                              Math::Mat4(Math::Vec4, Math::Vec4, Math::Vec4, Math::Vec4)>
                ctor;

            auto mul_overload =
                sol::overload([](const Math::Mat4& lhs, const Math::Mat4& rhs) -> Math::Mat4 { return lhs * rhs; },
                              [](const Math::Mat4& m, const float k) -> Math::Mat4 { return m * k; },
                              [](const float k, const Math::Mat4& m) -> Math::Mat4 { return k * m; });

            auto mat4 =
                lua.new_usertype<Math::Mat4>("Matrix4x4", ctor, sol::meta_function::multiplication, mul_overload);

            lua.set_function("translate", [](const Math::Mat4& matrix, const Math::Vec3& translation) {
                return translate(matrix, translation);
            });

            lua.set_function(
                "scale", [](const Math::Mat4& matrix, const Math::Vec3& scale) { return Math::Scale(matrix, scale); });

            lua.set_function("rotate", [](const Math::Mat4& matrix, const float angle, const Math::Vec3& axis) {
                return rotate(matrix, angle, axis);
            });

            lua.set_function("inverse", [](const Math::Mat4& matrix) { return inverse(matrix); });

            lua.set_function("transpose", [](const Math::Mat4& matrix) { return transpose(matrix); });
        }

        {
            sol::constructors<Math::Mat3(), Math::Mat3(float), Math::Mat3(Math::Vec3, Math::Vec3, Math::Vec3)> ctor;

            auto mul_overload =
                sol::overload([](const Math::Mat3& lhs, const Math::Mat3& rhs) -> Math::Mat4 { return lhs * rhs; },
                              [](const Math::Mat3& m, const float k) -> Math::Mat3 { return m * k; },
                              [](const float k, const Math::Mat3& m) -> Math::Mat3 { return k * m; });

            auto mat3 =
                lua.new_usertype<Math::Mat3>("Matrix3x3", ctor, sol::meta_function::multiplication, mul_overload);

            lua.set_function("inverse", [](const Math::Mat3& matrix) { return inverse(matrix); });

            lua.set_function("transpose", [](const Math::Mat3& matrix) { return transpose(matrix); });
        }

        {
            sol::constructors<Math::Quat(), Math::Quat(Math::Vec3), Math::Quat(float, Math::Vec3),
                              Math::Quat(float, float, float, float)>
                ctor;

            auto mul_overload =
                sol::overload([](const Math::Quat& lhs, const Math::Quat& rhs) -> Math::Quat { return lhs * rhs; },
                              [](const Math::Quat& q, const float k) -> Math::Quat { return q * k; },
                              [](const float k, const Math::Quat& q) -> Math::Quat { return k * q; });

            auto quat =
                lua.new_usertype<Math::Quat>("Quaternion", ctor, sol::meta_function::multiplication, mul_overload);

            quat.set_function("Euler", [](const Math::Quat& rot) { return eulerAngles(rot); });

            quat["x"] = &Math::Quat::x;
            quat["y"] = &Math::Quat::y;
            quat["z"] = &Math::Quat::z;
            quat["w"] = &Math::Quat::w;
        }

        lua.set_function("vector_mat4", &create_mat4_vector);
    }

} // namespace Vakol
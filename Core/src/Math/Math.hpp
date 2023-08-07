#pragma once

#include <glm/glm.hpp>

#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/gtc/quaternion.hpp>
#pragma warning(pop)

namespace Vakol::Math {
    //! glm usage is currently just here for backwards compatability reasons
    using Vec2 = glm::vec2;

    using Vec3 = glm::vec3;
    using Point = Vec3;

    using Mat3 = glm::mat3;
    using Mat4 = glm::mat4;

    using Quaternion = glm::quat;

    float Dot(const Vec2& a, const Vec2& b);
    float Dot(const Vec3& a, const Vec3& b);

    float Magnitude(const Vec2& v);
    float Magnitude(const Vec3& v);

    float MagnitudeSq(const Vec2& v);
    float MagnitudeSq(const Vec3& v);

    float Distance(const Point& p1, const Point& p2);

    void Normalize(Vec2& v);
    void Normalize(Vec3& v);
    Vec2 Normalized(const Vec2& v);
    Vec3 Normalized(const Vec3& v);

    Vec3 Cross(const Vec3& a, const Vec3& b);

    float Angle(const Vec2& a, const Vec2& b);
    float Angle(const Vec3& a, const Vec3& b);

    Vec2 Project(const Vec2& length, const Vec2& direction);
    Vec3 Project(const Vec3& length, const Vec3& direction);
    Vec2 Perpendicular(const Vec2& len, const Vec2& dir);
    Vec3 Perpendicular(const Vec3& len, const Vec3& dir);

    Mat4 Translation(const Vec3& pos);
    Mat3 Cut(const Mat4& mat, int row, int col);

    // assumes its degrees
    Mat4 XRotation(float angle);
    // assumes its degrees
    Mat4 YRotation(float angle);
    // assumes its degrees
    Mat4 ZRotation(float angle);

    Mat4 Rotation(float pitch, float yaw, float roll);

    Point MultiplyPoint(const Point& point, const Mat4& mat);
    Vec3 MultiplyVector(const Vec3& vec, const Mat4& mat);

    Mat4 Inverse(const Mat4& mat);

    struct Line
    {
        Point start;
        Point end;

        inline Line()
        {
        }
        inline Line(const Point& s, const Point& e) : start(s), end(e)
        {
        }
    };

    float Length(const Line& line);
    float LengthSq(const Line& line);

    struct Ray
    {
        Point origin;
        Vec3 dir;

        inline Ray() : dir(0.0f, 0.0f, 1.0f)
        {
        }
        inline Ray(const Point& o, const Vec3& d) : origin(o), dir(d)
        {
            NormalizeDirection();
        }
        inline void NormalizeDirection()
        {
            Normalize(dir);
        }
    };

    Ray FromPoints(const Point& from, const Point& to);

} // namespace Vakol::Math
#pragma once

#include <glm/glm.hpp>

namespace Vakol::Math {
    using Vec2 = glm::vec2;

    using Vec3 = glm::vec3;
    using Point = Vec3;

    using Mat4 = glm::mat4;

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

    struct Line {
        Point start;
        Point end;

        inline Line() {}
        inline Line(const Point& s, const Point& e) : start(s), end(e) {}
    };

    float Length(const Line& line);
    float LengthSq(const Line& line);

    struct Ray {
        Point origin;
        Vec3 dir;

        inline Ray() : dir(0.0f, 0.0f, 1.0f) {}
        inline Ray(const Point& o, const Vec3& d) : origin(o), dir(d) { NormalizeDirection(); }
        inline void NormalizeDirection() { Normalize(dir); }
    };

    Ray FromPoints(const Point& from, const Point& to);

}  // namespace Vakol::Math
#include "Math.hpp"

namespace Vakol::Math {

    float Dot(const Vec2& a, const Vec2& b) { return a.x * b.x + a.y * b.y; }
    float Dot(const Vec3& a, const Vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

    float Magnitude(const Vec2& v) { return sqrtf(Dot(v, v)); }
    float Magnitude(const Vec3& v) { return sqrtf(Dot(v, v)); }
    float MagnitudeSq(const Vec2& v) { return Dot(v, v); }
    float MagnitudeSq(const Vec3& v) { return Dot(v, v); }

    float Distance(const Point& p1, const Point& p2) {
        Vec3 t = p1 - p2;
        return Magnitude(t);
    }

    void Normalize(Vec2& v) { v = v * (1.0f / Magnitude(v)); }
    void Normalize(Vec3& v) { v = v * (1.0f / Magnitude(v)); }
    Vec2 Normalized(const Vec2& v) { return v * (1.0f / Magnitude(v)); }
    Vec3 Normalized(const Vec3& v) { return v * (1.0f / Magnitude(v)); }

    Vec3 Cross(const Vec3& a, const Vec3& b) {
        Vec3 res;
        res.x = a.y * b.z - a.z * b.y;
        res.y = a.z * b.x - a.x * b.z;
        res.z = a.x * b.y - a.y * b.x;
        return res;
    }

    float Angle(const Vec2& a, const Vec2& b) {
        float m = sqrtf(MagnitudeSq(a) * MagnitudeSq(b));
        return acos(Dot(a, b) / m);
    }
    float Angle(const Vec3& a, const Vec3& b) {
        float m = sqrtf(MagnitudeSq(a) * MagnitudeSq(b));
        return acos(Dot(a, b) / m);
    }

    float Length(const Line& line) { return Magnitude(line.start - line.end); }
    float LengthSq(const Line& line) { return MagnitudeSq(line.start - line.end); }

    Ray FromPoints(const Point& from, const Point& to) { return Ray(from, Normalized(to - from)); }
}  // namespace Vakol::Math
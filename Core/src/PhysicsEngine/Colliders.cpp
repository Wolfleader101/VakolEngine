#include "Colliders.hpp"

namespace Vakol::Physics {
    Vec3 GetMin(const AABB& aabb) {
        Vec3 p1 = aabb.pos + aabb.size;
        Vec3 p2 = aabb.pos - aabb.size;
        return Vec3(fminf(p1.x, p2.x), fminf(p1.y, p2.y), fminf(p1.z, p2.z));
    }
    Vec3 GetMax(const AABB& aabb) {
        Vec3 p1 = aabb.pos + aabb.size;
        Vec3 p2 = aabb.pos - aabb.size;
        return Vec3(fmaxf(p1.x, p2.x), fmaxf(p1.y, p2.y), fmaxf(p1.z, p2.z));
    }
    AABB FromMinMax(const Vec3& min, const Vec3& max) { return AABB((min + max) * 0.5f, (max - min) * 0.5f); }

    float PlaneEquation(const Point& point, const Plane& plane) { return Dot(point, plane.normal) - plane.dist; }
}  // namespace Vakol::Physics
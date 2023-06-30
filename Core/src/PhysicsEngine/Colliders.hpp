#pragma once

#include "Math/Math.hpp"

namespace Vakol::Physics {
    using namespace Vakol::Math;

    struct Sphere {
        Point pos;  //! origin
        float radius;

        inline Sphere() : radius(1.0f) {}
        inline Sphere(const Point& origin, float radius) : pos(origin), radius(radius) {}
    };

    struct AABB {
        Point pos;  //! origin
        Vec3 size;

        inline AABB() : size(1, 1, 1) {}
        inline AABB(const Point& origin, const Vec3& size) : pos(origin), size(size) {}
    };

    Vec3 GetMin(const AABB& aabb);
    Vec3 GetMax(const AABB& aabb);
    AABB FromMinMax(const Vec3& min, const Vec3& max);

    struct OBB {
        Point pos;
        Vec3 size;
        Mat3 orientation;

        inline OBB() : size(1, 1, 1) {}
        inline OBB(const Point& origin, const Vec3& size) : pos(origin), size(size) {}
        inline OBB(const Point& origin, const Vec3& size, const Mat3& o) : pos(origin), size(size), orientation(o) {}
    };

    struct Plane {
        Vec3 normal;
        float dist;
        inline Plane() : normal(1, 0, 0) {}
        inline Plane(const Vec3& norm, float dist) : normal(norm), dist(dist) {}
    };

    // 0 if the point is on the plane
    // positive number if the point is in front of the plane
    // negative number if the point is behind the plane
    float PlaneEquation(const Point& pt, const Plane& plane);

    struct Triangle {
        union {
            struct {
                Point a;
                Point b;
                Point c;
            };

            Point points[3];

            float values[9];
        };

        inline Triangle() {}
        inline Triangle(const Point& p1, const Point& p2, const Point& p3) : a(p1), b(p2), c(p3) {}
    };
}  // namespace Vakol::Physics
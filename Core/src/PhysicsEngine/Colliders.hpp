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

    struct Interval {
        float min;
        float max;
    };

    bool PointInSphere(const Point& point, const Sphere& sphere);
    Point ClosestPoint(const Sphere& sphere, const Point& point);

    bool PointInAABB(const Point& point, const AABB& aabb);
    Point ClosestPoint(const AABB& aabb, const Point& point);

    bool PointInOBB(const Point& point, const OBB& obb);
    Point ClosestPoint(const OBB& obb, const Point& point);

    bool PointOnPlane(const Point& point, const Plane& plane);
    Point ClosestPoint(const Plane& plane, const Point& point);

    bool PointOnLine(const Point& point, const Line& line);
    Point ClosestPoint(const Line& line, const Point& point);

    bool PointOnRay(const Point& point, const Ray& ray);
    Point ClosestPoint(const Ray& ray, const Point& point);

    bool SphereSphere(const Sphere& s1, const Sphere& s2);

    bool SphereAABB(const Sphere& sphere, const AABB& aabb);

    bool SphereOBB(const Sphere& sphere, const OBB& obb);

    bool SpherePlane(const Sphere& sphere, const Plane& plane);

    bool AABBAABB(const AABB& aabb1, const AABB& aabb2);

    Interval GetInterval(const AABB& aabb, const Vec3& axis);
    Interval GetInterval(const OBB& obb, const Vec3& axis);
    bool OverlapOnAxis(const AABB& aabb, const OBB& obb, const Vec3& axis);

    //! this follows Seperate Axis Theroem (SAT) for collision
    bool AABBOBB(const AABB& aabb, const OBB& obb);

    bool AABBPlane(const AABB& aabb, const Plane& plane);

    bool OverlapOnAxis(const OBB& obb1, const OBB& obb2, const Vec3& axis);

    //! this follows Seperate Axis Theroem (SAT) for collision
    bool OBBOBB(const OBB& obb1, const OBB& obb2);

    bool OBBPlane(const OBB& obb, const Plane& plane);

    bool PlanePlane(const Plane& plane1, const Plane& plane2);

    float Raycast(const Sphere& sphere, const Ray& ray);
    float Raycast(const AABB& aabb, const Ray& ray);
    float Raycast(const OBB& obb, const Ray& ray);
    float Raycast(const Plane& plane, const Ray& ray);

    bool Linetest(const Sphere& sphere, const Line& line);
    bool Linetest(const AABB& aabb, const Line& line);
    bool Linetest(const OBB& obb, const Line& line);
    bool Linetest(const Plane& plane, const Line& line);

    bool PointInTriangle(const Point& p, const Triangle& t);
}  // namespace Vakol::Physics
#include "Colliders.hpp"

#include <algorithm>
#include <list>
#include <stack>
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

    bool PointInSphere(const Point& point, const Sphere& sphere) {
        float magSq = MagnitudeSq(point - sphere.pos);
        float radSq = sphere.radius * sphere.radius;

        return magSq < radSq;
    }

    Point ClosestPoint(const Sphere& sphere, const Point& point) {
        Vec3 sphereToPoint = point - sphere.pos;
        Normalize(sphereToPoint);        // get normalized vector to test point
        sphereToPoint *= sphere.radius;  // resize to the size of radius

        return sphere.pos + sphereToPoint;  // offset it by the sphere pos
    }

    bool PointInAABB(const Point& point, const AABB& aabb) {
        Point min = GetMin(aabb);
        Point max = GetMax(aabb);

        if (point.x < min.x || point.y < min.y || point.z < min.z) {
            return false;
        }

        if (point.x > max.x || point.y > max.y || point.z > max.z) {
            return false;
        }

        return true;
    }

    Point ClosestPoint(const AABB& aabb, const Point& point) {
        Point min = GetMin(aabb);
        Point max = GetMax(aabb);

        Point result = point;

        // clamp min
        result.x = (result.x < min.x) ? min.x : result.x;
        result.y = (result.y < min.x) ? min.y : result.y;
        result.z = (result.z < min.x) ? min.z : result.z;

        // clamp max
        result.x = (result.x > max.x) ? max.x : result.x;
        result.y = (result.y > max.x) ? max.y : result.y;
        result.z = (result.z > max.x) ? max.z : result.z;
        return result;
    }

    bool PointInOBB(const Point& point, const OBB& obb) {  // move point
        Vec3 dir = point - obb.pos;

        // iterate over axis's x,y,z (each a vec3)
        for (int i = 0; i < 3; i++) {
            Vec3 axis = obb.orientation[i];

            // project point onto axis and get the distance from box origin to projection
            float distance = Dot(dir, axis);

            // if the distance is greater than the size, or less than the negative size, then point is not inside
            if (distance > obb.size[i]) {
                return false;
            }
            if (distance < -obb.size[i]) {
                return false;
            }
        }

        return true;
    }

    Point ClosestPoint(const OBB& obb, const Point& point) {
        Vec3 dir = point - obb.pos;
        Point result = obb.pos;

        // iterate over axis's x,y,z (each a vec3)
        for (int i = 0; i < 3; i++) {
            Vec3 axis = obb.orientation[i];

            // project point onto axis and get the distance from box origin to projection
            float distance = Dot(dir, axis);

            // if the distance is greater than the size, or less than the negative size, then clamp it
            if (distance > obb.size[i]) {
                distance = obb.size[i];
            }
            if (distance < -obb.size[i]) {
                distance = -obb.size[i];
            }

            // Adjust the final point by the axis and the current projected distance
            result += (axis * distance);
        }

        return result;
    }

    bool PointOnPlane(const Point& point, const Plane& plane) {
        //! assumes cpp does an epsilon check
        return PlaneEquation(point, plane) == 0.0f;
    }

    Point ClosestPoint(const Plane& plane, const Point& point) {
        float dot = Dot(plane.normal, point);
        float dist = dot - plane.dist;
        return point - plane.normal * dist;
    }

    bool PointOnLine(const Point& point, const Line& line) {
        Point closest = ClosestPoint(line, point);
        float distanceSq = MagnitudeSq(closest - point);

        return distanceSq == 0.0f;
    }

    Point ClosestPoint(const Line& line, const Point& point) {
        Vec3 lVec = line.end - line.start;  // Line Vector
        float t = Dot(point - line.start, lVec) / Dot(lVec, lVec);
        t = fmaxf(t, 0.0f);  // Clamp to 0
        t = fminf(t, 1.0f);  // Clamp to 1
        return line.start + lVec * t;
    }

    bool PointOnRay(const Point& point, const Ray& ray) {
        if (point == ray.origin) {
            return true;
        }

        Vec3 norm = point - ray.origin;
        Normalize(norm);

        float dot = Dot(norm, ray.origin);
        return dot == 1.0f;
    }

    Point ClosestPoint(const Ray& ray, const Point& point) {
        float t = Dot(point - ray.origin, ray.dir);
        // if ray direction is not normalized, uncomment the following line
        // t /= Dot(ray.dir, ray.dir);

        t = fmaxf(t, 0.0f);
        return Point(ray.origin + ray.dir * t);
    }

    bool SphereSphere(const Sphere& s1, const Sphere& s2) {
        float sum = s1.radius + s2.radius;

        float sqDist = MagnitudeSq(s1.pos - s2.pos);

        return sqDist < sum * sum;
    }

    bool SphereAABB(const Sphere& sphere, const AABB& aabb) {
        Point closest = ClosestPoint(aabb, sphere.pos);
        float dist = MagnitudeSq(sphere.pos - closest);

        return dist < sphere.radius * sphere.radius;
    }

    bool SphereOBB(const Sphere& sphere, const OBB& obb) {
        Point closest = ClosestPoint(obb, sphere.pos);
        float dist = MagnitudeSq(sphere.pos - closest);

        return dist < sphere.radius * sphere.radius;
    }

    bool SpherePlane(const Sphere& sphere, const Plane& plane) {
        Point closest = ClosestPoint(plane, sphere.pos);
        float dist = MagnitudeSq(sphere.pos - closest);

        return dist < sphere.radius * sphere.radius;
    }

    bool AABBAABB(const AABB& aabb1, const AABB& aabb2) {
        Point aMin = GetMin(aabb1);
        Point aMax = GetMax(aabb1);

        Point bMin = GetMin(aabb2);
        Point bMax = GetMax(aabb2);

        return (aMin.x <= bMax.x && aMax.x >= bMin.x) && (aMin.y <= bMax.y && aMax.y >= bMin.y) &&
               (aMin.z <= bMax.z && aMax.z >= bMin.z);
    }

    Interval GetInterval(const AABB& aabb, const Vec3& axis) {
        Vec3 min = GetMin(aabb);
        Vec3 max = GetMax(aabb);

        Vec3 vertex[8] = {Vec3(min.x, max.y, max.z), Vec3(min.x, max.y, min.z), Vec3(min.x, min.y, max.z),
                          Vec3(min.x, min.y, min.z), Vec3(max.x, max.y, max.z), Vec3(max.x, max.y, min.z),
                          Vec3(max.x, min.y, max.z), Vec3(max.x, min.y, min.z)};
        Interval result;
        result.min = result.max = Dot(axis, vertex[0]);
        for (int i = 1; i < 8; ++i) {
            float projection = Dot(axis, vertex[i]);
            result.min = (projection < result.min) ? projection : result.min;
            result.max = (projection > result.max) ? projection : result.max;
        }
        return result;
    }

    Interval GetInterval(const OBB& obb, const Vec3& axis) {
        Vec3 vertex[8];

        // OBB Center
        Vec3 C = obb.pos;
        // OBB Extents
        Vec3 E = obb.size;
        // OBB Axis
        Vec3 A[] = {obb.orientation[0], obb.orientation[1], obb.orientation[2]};

        vertex[0] = C + A[0] * E[0] + A[1] * E[1] + A[2] * E[2];
        vertex[1] = C - A[0] * E[0] + A[1] * E[1] + A[2] * E[2];
        vertex[2] = C + A[0] * E[0] - A[1] * E[1] + A[2] * E[2];
        vertex[3] = C + A[0] * E[0] + A[1] * E[1] - A[2] * E[2];
        vertex[4] = C - A[0] * E[0] - A[1] * E[1] - A[2] * E[2];
        vertex[5] = C + A[0] * E[0] - A[1] * E[1] - A[2] * E[2];
        vertex[6] = C - A[0] * E[0] + A[1] * E[1] - A[2] * E[2];
        vertex[7] = C - A[0] * E[0] - A[1] * E[1] + A[2] * E[2];

        Interval result;
        result.min = result.max = Dot(axis, vertex[0]);
        for (int i = 1; i < 8; ++i) {
            float projection = Dot(axis, vertex[i]);
            result.min = (projection < result.min) ? projection : result.min;
            result.max = (projection > result.max) ? projection : result.max;
        }
        return result;
    }

    bool OverlapOnAxis(const AABB& aabb, const OBB& obb, const Vec3& axis) {
        Interval a = GetInterval(aabb, axis);
        Interval b = GetInterval(obb, axis);
        return ((b.min <= a.max) && (a.min <= b.max));
    }

    bool AABBOBB(const AABB& aabb, const OBB& obb) {
        Vec3 test[15] = {
            Vec3(1, 0, 0),       // AABB axis 1
            Vec3(0, 1, 0),       // AABB axis 2
            Vec3(0, 0, 1),       // AABB axis 3
            obb.orientation[0],  // OBB axis 1
            obb.orientation[1],  // OBB axis 2
            obb.orientation[2],  // OBB axis 3
        };

        // The next nine axes are the cross products of the rotation frames of the two shapes
        for (int i = 0; i < 3; ++i) {
            test[6 + i * 3 + 0] = Cross(test[i], test[0]);
            test[6 + i * 3 + 1] = Cross(test[i], test[1]);
            test[6 + i * 3 + 2] = Cross(test[i], test[2]);
        }

        for (int i = 0; i < 15; ++i) {
            if (!OverlapOnAxis(aabb, obb, test[i])) {
                return false;  // Seperating axis found
            }
        }
        return true;  // Seperating axis not found
    }

    bool AABBPlane(const AABB& aabb, const Plane& plane) {
        // Project the half extents of the AABB onto the plane normal
        float pLen = aabb.size.x * fabsf(plane.normal.x) + aabb.size.y * fabsf(plane.normal.y) +
                     aabb.size.z * fabsf(plane.normal.z);

        //  Find the distance from the center of the AABB to the plane:
        float dot = Dot(plane.normal, aabb.pos);
        float dist = dot - plane.dist;

        return fabsf(dist) <= pLen;
    }

    bool OverlapOnAxis(const OBB& obb1, const OBB& obb2, const Vec3& axis) {
        Interval a = GetInterval(obb1, axis);
        Interval b = GetInterval(obb2, axis);
        return ((b.min <= a.max) && (a.min <= b.max));
    }

    bool OBBOBB(const OBB& obb1, const OBB& obb2) {
        Vec3 test[15] = {
            obb1.orientation[0],  // OBB1 axis 1
            obb1.orientation[1],  // OBB1 axis 2
            obb1.orientation[2],  // OBB1 axis 3

            obb2.orientation[0],  // OBB2 axis 1
            obb2.orientation[1],  // OBB2 axis 2
            obb2.orientation[2],  // OBB2 axis 3
        };

        for (int i = 0; i < 3; ++i) {
            test[6 + i * 3 + 0] = Cross(test[i], test[0]);
            test[6 + i * 3 + 1] = Cross(test[i], test[1]);
            test[6 + i * 3 + 2] = Cross(test[i], test[2]);
        }

        for (int i = 0; i < 15; ++i) {
            if (!OverlapOnAxis(obb1, obb2, test[i])) {
                return false;  // Seperating axis found
            }
        }
        return true;  // Seperating axis not found
    }

    bool OBBPlane(const OBB& obb, const Plane& plane) {
        Vec3 rot[] = {
            // rotation / orientation
            obb.orientation[0],
            obb.orientation[1],
            obb.orientation[2],
        };
        Vec3 normal = plane.normal;

        float pLen = obb.size.x * fabsf(Dot(normal, rot[0])) + obb.size.y * fabsf(Dot(normal, rot[1])) +
                     obb.size.z * fabsf(Dot(normal, rot[2]));

        float dot = Dot(plane.normal, obb.pos);
        float dist = dot - plane.dist;

        return fabsf(dist) <= pLen;
    }

    bool PlanePlane(const Plane& plane1, const Plane& plane2) {
        // Cross product returns 0 when used on parallel lines
        Vec3 c = Cross(plane1.normal, plane2.normal);

        return Dot(c, c) != 0.0f;

        /*
        ! if the plane normals are normalised then you could just do:
        return Dot(plane1.normal, plane2.normal) != 1.0f; // (assumes they are normalised)
        */
    }

    float Raycast(const Sphere& sphere, const Ray& ray) {
        Vec3 e = sphere.pos - ray.origin;
        float rSq = sphere.radius * sphere.radius;
        float eSq = MagnitudeSq(e);

        // ray dir is assumed to be normalized
        float a = Dot(e, ray.dir);

        float bSq = eSq - (a * a);
        float f = sqrt(rSq - bSq);

        // No collision has happened
        if (rSq - (eSq - (a * a)) < 0.0f) {
            return -1;  // -1 is invalid.
        }
        // Ray starts inside the sphere
        else if (eSq < rSq) {
            return a + f;  // Just reverse direction
        }
        // else Normal intersection
        return a - f;
    }

    float Raycast(const AABB& aabb, const Ray& ray) {
        Vec3 min = GetMin(aabb);
        Vec3 max = GetMax(aabb);

        float t1 = (min.x - ray.origin.x) / ray.dir.x;
        float t2 = (max.x - ray.origin.x) / ray.dir.x;
        float t3 = (min.y - ray.origin.y) / ray.dir.y;
        float t4 = (max.y - ray.origin.y) / ray.dir.y;
        float t5 = (min.z - ray.origin.z) / ray.dir.z;
        float t6 = (max.z - ray.origin.z) / ray.dir.z;

        //! find the biggest min
        float tmin = fmaxf(fmaxf(fminf(t1, t2), fminf(t3, t4)), fminf(t5, t6));

        //! final the smallest max
        float tmax = fminf(fminf(fmaxf(t1, t2), fmaxf(t3, t4)), fmaxf(t5, t6));

        // If tmax is less than zero, the ray is intersecting AABB behind the origin of the ray, this should not be
        // treated as an intersection
        if (tmax < 0.0f) {
            return -1;
        }

        // If tmin is greater than tmax, the ray does not intersect AABB
        if (tmin > tmax) {
            return -1;
        }

        // If tmin is less than zero, that means the ray intersects the AABB but its origin is
        // inside the AABB. This means tmax is the valid collision point:
        if (tmin < 0.0f) {
            return tmax;
        }

        return tmin;
    }

    float Raycast(const OBB& obb, const Ray& ray) {
        // X, Y and Z axis of OBB
        Vec3 X = obb.orientation[0];
        Vec3 Y = obb.orientation[1];
        Vec3 Z = obb.orientation[2];

        // vector pointing from the origin of the ray to the OBB
        Vec3 p = obb.pos - ray.origin;

        // project the direction of the ray onto each of the axis of the OBB
        Vec3 f(Dot(X, ray.dir), Dot(Y, ray.dir), Dot(Z, ray.dir));

        // project p into every axis of the OBBs rotation frame
        Vec3 e(Dot(X, p), Dot(Y, p), Dot(Z, p));

        // tmin,max for x,y,z
        float t[6] = {0, 0, 0, 0, 0, 0};
        for (int i = 0; i < 3; ++i) {
            if (f[i] == 0.0f) {
                if (-e[i] - obb.size[i] > 0 || -e[i] + obb.size[i] < 0) {
                    return -1;
                }
                f[i] = 0.00001f;  //! Avoid div by 0
            }
            t[i * 2 + 0] = (e[i] + obb.size[i]) / f[i];  // min
            t[i * 2 + 1] = (e[i] - obb.size[i]) / f[i];  // max
        }

        //! find the biggest min
        float tmin = fmaxf(fmaxf(fminf(t[0], t[1]), fminf(t[2], t[3])), fminf(t[4], t[5]));

        //! final the smallest max
        float tmax = fminf(fminf(fmaxf(t[0], t[1]), fmaxf(t[2], t[3])), fmaxf(t[4], t[5]));

        // If tmax is less than zero, the ray is intersecting AABB behind the origin of the ray, this should not be
        // treated as an intersection
        if (tmax < 0) {
            return -1.0f;
        }

        // If tmin is greater than tmax, the ray does not intersect AABB
        if (tmin > tmax) {
            return -1.0f;
        }

        // If tmin is less than zero, that means the ray intersects the AABB but its origin is
        // inside the AABB. This means tmax is the valid collision point:
        if (tmin < 0.0f) {
            return tmax;
        }
        return tmin;
    }

    float Raycast(const Plane& plane, const Ray& ray) {
        float nd = Dot(ray.dir, plane.normal);
        float pn = Dot(ray.origin, plane.normal);

        // ff nd is positive or 0, the ray and plane normals point in the same direction
        if (nd >= 0.0f) {
            return -1;
        }

        // if t is negative, the ray hits the plane behind its origin
        float t = (plane.dist - pn) / nd;

        if (t >= 0.0f) {
            return t;
        }
        return -1;
    }

    bool Linetest(const Sphere& sphere, const Line& line) {
        Point closest = ClosestPoint(line, sphere.pos);

        // get distance between sphere pos and closest point (use squared as its faster)
        float distSq = MagnitudeSq(sphere.pos - closest);

        // if its smaller than the radius squareds, it intersects
        return distSq <= (sphere.radius * sphere.radius);
    }

    bool Linetest(const AABB& aabb, const Line& line) {
        // construct a ray, where origin is start, and directon is normalized
        Ray ray;
        ray.origin = line.start;
        ray.dir = Normalized(line.end - line.start);
        float t = Raycast(aabb, ray);

        // check that the raycast is less then the length of the line squared
        return t >= 0 && t * t <= LengthSq(line);
    }

    bool Linetest(const OBB& obb, const Line& line) {
        // construct a ray, where origin is start, and directon is normalized
        Ray ray;
        ray.origin = line.start;
        ray.dir = Normalized(line.end - line.start);
        float t = Raycast(obb, ray);

        // check that the raycast is less then the length of the line squared
        return t >= 0 && t * t <= LengthSq(line);
    }

    bool Linetest(const Plane& plane, const Line& line) {
        Vec3 ab = line.end - line.start;
        float nA = Dot(plane.normal, line.start);
        float nAB = Dot(plane.normal, ab);

        // If the line and plane are parallel, nAB will be 0
        if (nAB == 0.0f) return false;

        float t = (plane.dist - nA) / nAB;
        return t >= 0.0f && t <= 1.0f;
    }

    bool PointInTriangle(const Point& p, const Triangle& t) {
        Vec3 a = t.a - p;
        Vec3 b = t.b - p;
        Vec3 c = t.c - p;

        // The point should be moved too, so they are both
        // relative, but because we don't use p in the
        // equation anymore, we don't need it!
        // p -= p; This would just equal the zero vector!

        // Given point P and triangle ABC, create the sides of a pyramid. The sides of the
        // pyramid will be triangles created from the points: PBC, PCA, PAB. Then, find and
        // store the normal of each side of this pyramid:
        Vec3 normPBC = Cross(b, c);  // Normal of PBC (u)
        Vec3 normPCA = Cross(c, a);  // Normal of PCA (v)
        Vec3 normPAB = Cross(a, b);  // Normal of PAB (w)

        // If the faces of the pyramid do not have the same normal, the point is not in the triangle
        if (Dot(normPBC, normPCA) < 0.0f) {
            return false;
        } else if (Dot(normPBC, normPAB) < 0.0f) {
            return false;
        }

        return true;
    }

    Plane FromTriangle(const Triangle& t) {
        Plane result;
        result.normal = Normalized(Cross(t.b - t.a, t.c - t.a));

        result.dist = Dot(result.normal, t.a);

        return result;
    }

    Point ClosestPoint(const Triangle& t, const Point& p) {
        Plane plane = FromTriangle(t);

        Point closest = ClosestPoint(plane, p);

        // if its inside the triangle
        if (PointInTriangle(closest, t)) {
            return closest;
        }

        // line for each side of triangle
        Point c1 = ClosestPoint(Line(t.a, t.b), p);  // Line AB
        Point c2 = ClosestPoint(Line(t.b, t.c), p);  // Line BC
        Point c3 = ClosestPoint(Line(t.c, t.a), p);  // Line CA

        float magSq1 = MagnitudeSq(p - c1);
        float magSq2 = MagnitudeSq(p - c2);
        float magSq3 = MagnitudeSq(p - c3);

        if (magSq1 < magSq2 && magSq1 < magSq3) {
            return c1;
        } else if (magSq2 < magSq1 && magSq2 < magSq3) {
            return c2;
        }
        return c3;
    }

    bool TriangleSphere(const Triangle& t, const Sphere& s) {
        Point closest = ClosestPoint(t, s.pos);
        float magSq = MagnitudeSq(closest - s.pos);
        return magSq <= s.radius * s.radius;
    }

    Interval GetInterval(const Triangle& triangle, const Vec3& axis) {
        Interval result;
        // project first point and store it as min and max
        result.min = Dot(axis, triangle.points[0]);
        result.max = result.min;

        for (int i = 1; i < 3; ++i) {
            float value = Dot(axis, triangle.points[i]);
            result.min = fminf(result.min, value);
            result.max = fmaxf(result.max, value);
        }
        return result;
    }

    bool OverlapOnAxis(const AABB& aabb, const Triangle& triangle, const Vec3& axis) {
        Interval a = GetInterval(aabb, axis);
        Interval b = GetInterval(triangle, axis);
        return ((b.min <= a.max) && (a.min <= b.max));
    }

    bool TriangleAABB(const Triangle& t, const AABB& a) {
        // edge vectors of ABC
        Vec3 f0 = t.b - t.a;
        Vec3 f1 = t.c - t.b;
        Vec3 f2 = t.a - t.c;

        // face normals of aabb
        Vec3 u0(1.0f, 0.0f, 0.0f);
        Vec3 u1(0.0f, 1.0f, 0.0f);
        Vec3 u2(0.0f, 0.0f, 1.0f);

        Vec3 test[13] = {u0,             // AABB Axis 1
                         u1,             // AABB Axis 2
                         u2,             // AABB Axis 3
                         Cross(f0, f1),  // normal of the triangle
                         Cross(u0, f0),
                         Cross(u0, f1),
                         Cross(u0, f2),
                         Cross(u1, f0),
                         Cross(u1, f1),
                         Cross(u1, f2),
                         Cross(u2, f0),
                         Cross(u2, f1),
                         Cross(u2, f2)};
        // ^^all normals of the AABB with all edges of triangle

        for (int i = 0; i < 13; ++i) {
            if (!OverlapOnAxis(a, t, test[i])) {
                return false;  // Separating axis found
            }
        }

        return true;  // Separating axis not found
    }

    bool OverlapOnAxis(const OBB& obb, const Triangle& triangle, const Vec3& axis) {
        Interval a = GetInterval(obb, axis);
        Interval b = GetInterval(triangle, axis);
        return ((b.min <= a.max) && (a.min <= b.max));
    }

    bool TriangleOBB(const Triangle& t, const OBB& o) {
        //  edge vectors of triangle (ABC)
        Vec3 f0 = t.b - t.a;
        Vec3 f1 = t.c - t.b;
        Vec3 f2 = t.a - t.c;

        Vec3 u0(o.orientation[0]);
        Vec3 u1(o.orientation[1]);
        Vec3 u2(o.orientation[2]);

        Vec3 test[13] = {u0,             // OBB Axis 1
                         u1,             // OBB Axis 2
                         u2,             // OBB Axis 3
                         Cross(f0, f1),  // Normal of the Triangle
                         Cross(u0, f0),
                         Cross(u0, f1),
                         Cross(u0, f2),
                         Cross(u1, f0),
                         Cross(u1, f1),
                         Cross(u1, f2),
                         Cross(u2, f0),
                         Cross(u2, f1),
                         Cross(u2, f2)};

        for (int i = 0; i < 13; ++i) {
            if (!OverlapOnAxis(o, t, test[i])) {
                return false;  // Separating axis found
            }
        }

        return true;  // Separating axis not found
    }

    bool TrianglePlane(const Triangle& t, const Plane& p) {
        float side1 = PlaneEquation(t.a, p);
        float side2 = PlaneEquation(t.b, p);
        float side3 = PlaneEquation(t.c, p);

        // if all points are on the plane
        if (side1 == 0.0f && side2 == 0.0f && side3 == 0.0f) {
            return true;
        }

        // if in front of the plane
        if (side1 > 0.0f && side2 > 0.0f && side3 > 0.0f) {
            return false;
        }

        // if behind the plane
        if (side1 < 0.0f && side2 < 0.0f && side3 < 0.0f) {
            return false;
        }

        // else one of the sides is intersecting
        return true;
    }

    bool OverlapOnAxis(const Triangle& t1, const Triangle& t2, const Vec3& axis) {
        Interval a = GetInterval(t1, axis);
        Interval b = GetInterval(t2, axis);
        return ((b.min <= a.max) && (a.min <= b.max));
    }

    Vec3 SatCrossEdge(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& d) {
        Vec3 ab = a - b;
        Vec3 cd = c - d;
        Vec3 result = Cross(ab, cd);

        // cross product returns 0 vector if parralel, if its not parralel return it
        if (MagnitudeSq(result) != 0.0f) {
            return result;
        } else {  // in the case of being parralel, check perpendicular axis
            Vec3 axis = Cross(ab, c - a);
            result = Cross(ab, axis);

            // if not parralel
            if (MagnitudeSq(result) != 0.0f) {
                return result;
            }
        }

        return Vec3(0.0f);
    }

    bool TriangleTriangle(const Triangle& t1, const Triangle& t2) {
        Vec3 axisToTest[11] = {
            SatCrossEdge(t1.a, t1.b, t1.b, t1.c),  // triangle 1 normal
            SatCrossEdge(t2.a, t2.b, t2.b, t2.c),  // triangle 2 normal
            SatCrossEdge(t2.a, t2.b, t1.a, t1.b), SatCrossEdge(t2.a, t2.b, t1.b, t1.c),
            SatCrossEdge(t2.a, t2.b, t1.c, t1.a), SatCrossEdge(t2.b, t2.c, t1.a, t1.b),
            SatCrossEdge(t2.b, t2.c, t1.b, t1.c), SatCrossEdge(t2.b, t2.c, t1.c, t1.a),
            SatCrossEdge(t2.c, t2.a, t1.a, t1.b), SatCrossEdge(t2.c, t2.a, t1.b, t1.c),
            SatCrossEdge(t2.c, t2.a, t1.c, t1.a),

        };

        for (int i = 0; i < 11; ++i) {
            if (!OverlapOnAxis(t1, t2, axisToTest[i])) {
                if (MagnitudeSq(axisToTest[i]) == 0.0f) {
                    return false;  // Seperating axis found
                }
            }
        }
        return true;
    }

    Vec3 Barycentric(const Point& p, const Triangle& t) {
        // test point to each triangle point
        Vec3 ap = p - t.a;
        Vec3 bp = p - t.b;
        Vec3 cp = p - t.c;

        // edges of triangle
        Vec3 ab = t.b - t.a;
        Vec3 ac = t.c - t.a;
        Vec3 bc = t.c - t.b;
        Vec3 cb = t.b - t.c;
        Vec3 ca = t.a - t.c;

        // perpendicular to AB
        Vec3 v = Perpendicular(ab, ac);
        // perpendicular function looks like: ab - Project(ab, cb);

        // is 0 if the projected point is on line AB. value is 1 if the projected point is at C
        float a = 1.0f - (Dot(v, ap) / Dot(v, ab));

        // perpenndicular to to BC
        v = Perpendicular(bc, ac);
        float b = 1.0f - (Dot(v, bp) / Dot(v, bc));

        // perpendicular to CA
        v = Perpendicular(ca, ab);
        float c = 1.0f - (Dot(v, cp) / Dot(v, ca));

        return Vec3(a, b, c);
    }

    float Raycast(const Triangle& triangle, const Ray& ray) {
        Plane plane = FromTriangle(triangle);
        float t = Raycast(plane, ray);

        // if it does not hit the plane return
        if (t < 0.0f) {
            return t;
        }

        // find where it hit
        Point result = ray.origin + ray.dir * t;

        // find barbycentric coordinates
        Vec3 barycentric = Barycentric(result, triangle);
        if (barycentric.x >= 0.0f && barycentric.x <= 1.0f && barycentric.y >= 0.0f && barycentric.y <= 1.0f &&
            barycentric.z >= 0.0f && barycentric.z <= 1.0f) {
            return t;
        }

        return -1;
    }

    bool Linetest(const Triangle& triangle, const Line& line) {
        Ray ray;
        ray.origin = line.start;
        ray.dir = Normalized(line.end - line.start);

        float t = Raycast(triangle, ray);

        // check that the raycast is within the limits of the line
        return t >= 0 && t * t <= LengthSq(line);
    }

    void AccelerateMesh(Mesh& mesh) {
        // assume a BVH has already been created
        if (mesh.accelerator != nullptr) {
            return;
        }

        // find min and max points of meh
        Vec3 min = mesh.vertices[0];
        Vec3 max = mesh.vertices[0];
        for (int i = 1; i < mesh.numTriangles * 3; ++i) {
            min.x = fminf(mesh.vertices[i].x, min.x);
            min.y = fminf(mesh.vertices[i].y, min.y);
            min.z = fminf(mesh.vertices[i].z, min.z);
            max.x = fmaxf(mesh.vertices[i].x, max.x);
            max.y = fmaxf(mesh.vertices[i].y, max.y);
            max.z = fmaxf(mesh.vertices[i].z, max.z);
        }

        // create new bvh
        mesh.accelerator = new BVHNode();  //! WARNING this is unsafe if not treated carefully
        mesh.accelerator->bounds = FromMinMax(min, max);
        mesh.accelerator->numTriangles = mesh.numTriangles;
        mesh.accelerator->triangles = new int[mesh.numTriangles];

        for (int i = 0; i < mesh.numTriangles; ++i) {
            mesh.accelerator->triangles[i] = i;
        }

        SplitBVHNode(mesh.accelerator, mesh, 3);
    }

    void SplitBVHNode(BVHNode* node, const Mesh& model, int depth) {
        // return if depth has been reached
        if (depth-- == 0) {
            return;
        }

        // if its a leaf node split it into 8 child nodes
        if (node->children == nullptr && node->numTriangles > 0) {
            node->children = new BVHNode[8];

            // set the bounding box of each child node (center is parent center)
            Vec3 c = node->bounds.pos;
            Vec3 e = node->bounds.size * 0.5f;
            node->children[0].bounds = AABB(c + Vec3(-e.x, +e.y, -e.z), e);
            node->children[1].bounds = AABB(c + Vec3(+e.x, +e.y, -e.z), e);
            node->children[2].bounds = AABB(c + Vec3(-e.x, +e.y, +e.z), e);
            node->children[3].bounds = AABB(c + Vec3(+e.x, +e.y, +e.z), e);
            node->children[4].bounds = AABB(c + Vec3(-e.x, -e.y, -e.z), e);
            node->children[5].bounds = AABB(c + Vec3(+e.x, -e.y, -e.z), e);
            node->children[6].bounds = AABB(c + Vec3(-e.x, -e.y, +e.z), e);
            node->children[7].bounds = AABB(c + Vec3(+e.x, -e.y, +e.z), e);
        }

        // if the node was split
        if (node->children != nullptr && node->numTriangles > 0) {
            // for each child check how many children it should contain
            for (int i = 0; i < 8; i++) {
                node->children[i].numTriangles = 0;

                // figure out how many children it should contain
                for (int j = 0; j < node->numTriangles; ++j) {
                    Triangle t = model.triangles[node->triangles[j]];

                    // if it intersects increase its triangle count
                    if (TriangleAABB(t, node->children[i].bounds)) node->children[i].numTriangles += 1;
                }

                // if the child node does not contain any triangles then continue
                if (node->children[i].numTriangles == 0) continue;

                // allocate new memory for indices in child node
                node->children[i].triangles = new int[node->children[i].numTriangles];
                int index = 0;

                // for any triangle that intersects the child node, add it's index to the triangle indices
                for (int j = 0; j < node->numTriangles; ++j) {
                    Triangle t = model.triangles[node->triangles[j]];
                    if (TriangleAABB(t, node->children[i].bounds))
                        node->children[i].triangles[index++] = node->triangles[j];
                }
            }

            node->numTriangles = 0;
            delete[] node->triangles;
            node->triangles = 0;

            for (int i = 0; i < 8; i++) {
                SplitBVHNode(&node->children[i], model, depth);
            }
        }
    }

    void FreeBVHNode(BVHNode* node) {
        if (node->children == nullptr) return;

        for (int i = 0; i < 8; i++) {
            FreeBVHNode(&node->children[i]);
        }

        delete[] node->children;
        node->children = nullptr;

        if (node->numTriangles != 0 || node->triangles != nullptr) {
            delete[] node->triangles;
            node->triangles = nullptr;
            node->numTriangles = 0;
        }
    }

    float MeshRay(const Mesh& mesh, const Ray& ray) {
        if (mesh.accelerator == nullptr) {
            for (int i = 0; i < mesh.numTriangles; i++) {
                float res = Raycast(mesh.triangles[i], ray);
                if (res >= 0.0f) return res;
            }
            return -1.0f;
        }

        // else it has a BVH
        std::list<BVHNode*> nodesToCheck;
        nodesToCheck.push_front(mesh.accelerator);
        while (!nodesToCheck.empty()) {
            BVHNode* it = *(nodesToCheck.begin());
            nodesToCheck.erase(nodesToCheck.begin());

            // if its a leaf node
            if (it->numTriangles >= 0) {
                for (int i = 0; i < it->numTriangles; ++i) {
                    // Do a raycast against the triangle
                    float r = Raycast(mesh.triangles[it->triangles[i]], ray);
                    if (r >= 0) {
                        return r;
                    }
                }
            }

            // if its a parent node
            if (it->children != nullptr) {
                for (int i = 8 - 1; i >= 0; --i) {
                    if (Raycast(it->bounds, ray) >= 0.0f) {
                        nodesToCheck.push_front(&it->children[i]);
                    }
                }
            }
        }

        // if all else fails return -1
        return -1.0f;
    }

    bool MeshAABB(const Mesh& mesh, const AABB& aabb) {
        if (mesh.accelerator == nullptr) {
            // brute force our way through the triangles
            for (int i = 0; i < mesh.numTriangles; ++i) {
                if (TriangleAABB(mesh.triangles[i], aabb)) {
                    return true;
                }
            }
            return false;
        }
        // else it has a BVH
        std::list<BVHNode*> nodesToCheck;
        nodesToCheck.push_front(mesh.accelerator);
        while (!nodesToCheck.empty()) {
            BVHNode* it = *(nodesToCheck.begin());
            nodesToCheck.erase(nodesToCheck.begin());

            // if its a leaf node
            if (it->numTriangles >= 0) {
                for (int i = 0; i < it->numTriangles; ++i) {
                    // check if it collides
                    if (TriangleAABB(mesh.triangles[it->triangles[i]], aabb)) {
                        return true;
                    }
                }
            }

            // if its a parent node
            if (it->children != nullptr) {
                for (int i = 8 - 1; i >= 0; --i) {
                    // if it collides then add it to the list
                    if (AABBAABB(it->children[i].bounds, aabb)) {
                        nodesToCheck.push_front(&it->children[i]);
                    }
                }
            }
        }

        return false;
    }

    bool Linetest(const Mesh& mesh, const Line& line) {
        if (mesh.accelerator == nullptr) {
            // brute force our way through the triangles
            for (int i = 0; i < mesh.numTriangles; ++i) {
                if (Linetest(mesh.triangles[i], line)) {
                    return true;
                }
            }
            return false;
        }
        // else it has a BVH
        std::list<BVHNode*> nodesToCheck;
        nodesToCheck.push_front(mesh.accelerator);
        while (!nodesToCheck.empty()) {
            BVHNode* it = *(nodesToCheck.begin());
            nodesToCheck.erase(nodesToCheck.begin());

            // if its a leaf node
            if (it->numTriangles >= 0) {
                for (int i = 0; i < it->numTriangles; ++i) {
                    // check if it collides
                    if (Linetest(mesh.triangles[it->triangles[i]], line)) {
                        return true;
                    }
                }
            }

            // if its a parent node
            if (it->children != nullptr) {
                for (int i = 8 - 1; i >= 0; --i) {
                    // if it collides then add it to the list
                    if (Linetest(it->children[i].bounds, line)) {
                        nodesToCheck.push_front(&it->children[i]);
                    }
                }
            }
        }

        return false;
    }

    bool MeshSphere(const Mesh& mesh, const Sphere& sphere) {
        if (mesh.accelerator == nullptr) {
            // brute force our way through the triangles
            for (int i = 0; i < mesh.numTriangles; ++i) {
                if (TriangleSphere(mesh.triangles[i], sphere)) {
                    return true;
                }
            }
            return false;
        }
        // else it has a BVH
        std::list<BVHNode*> nodesToCheck;
        nodesToCheck.push_front(mesh.accelerator);
        while (!nodesToCheck.empty()) {
            BVHNode* it = *(nodesToCheck.begin());
            nodesToCheck.erase(nodesToCheck.begin());

            // if its a leaf node
            if (it->numTriangles >= 0) {
                for (int i = 0; i < it->numTriangles; ++i) {
                    // check if it collides
                    if (TriangleSphere(mesh.triangles[it->triangles[i]], sphere)) {
                        return true;
                    }
                }
            }

            // if its a parent node
            if (it->children != nullptr) {
                for (int i = 8 - 1; i >= 0; --i) {
                    // if it collides then add it to the list
                    if (AABBSphere(it->children[i].bounds, sphere)) {
                        nodesToCheck.push_front(&it->children[i]);
                    }
                }
            }
        }

        return false;
    }

    bool MeshOBB(const Mesh& mesh, const OBB& obb) {
        if (mesh.accelerator == nullptr) {
            // brute force our way through the triangles
            for (int i = 0; i < mesh.numTriangles; ++i) {
                if (TriangleOBB(mesh.triangles[i], obb)) {
                    return true;
                }
            }
            return false;
        }
        // else it has a BVH
        std::list<BVHNode*> nodesToCheck;
        nodesToCheck.push_front(mesh.accelerator);
        while (!nodesToCheck.empty()) {
            BVHNode* it = *(nodesToCheck.begin());
            nodesToCheck.erase(nodesToCheck.begin());

            // if its a leaf node
            if (it->numTriangles >= 0) {
                for (int i = 0; i < it->numTriangles; ++i) {
                    // check if it collides
                    if (TriangleOBB(mesh.triangles[it->triangles[i]], obb)) {
                        return true;
                    }
                }
            }

            // if its a parent node
            if (it->children != nullptr) {
                for (int i = 8 - 1; i >= 0; --i) {
                    // if it collides then add it to the list
                    if (AABBOBB(it->children[i].bounds, obb)) {
                        nodesToCheck.push_front(&it->children[i]);
                    }
                }
            }
        }

        return false;
    }

    bool MeshPlane(const Mesh& mesh, const Plane& plane) {
        if (mesh.accelerator == nullptr) {
            // brute force our way through the triangles
            for (int i = 0; i < mesh.numTriangles; ++i) {
                if (TrianglePlane(mesh.triangles[i], plane)) {
                    return true;
                }
            }
            return false;
        }
        // else it has a BVH
        std::list<BVHNode*> nodesToCheck;
        nodesToCheck.push_front(mesh.accelerator);
        while (!nodesToCheck.empty()) {
            BVHNode* it = *(nodesToCheck.begin());
            nodesToCheck.erase(nodesToCheck.begin());

            // if its a leaf node
            if (it->numTriangles >= 0) {
                for (int i = 0; i < it->numTriangles; ++i) {
                    // check if it collides
                    if (TrianglePlane(mesh.triangles[it->triangles[i]], plane)) {
                        return true;
                    }
                }
            }

            // if its a parent node
            if (it->children != nullptr) {
                for (int i = 8 - 1; i >= 0; --i) {
                    // if it collides then add it to the list
                    if (AABBPlane(it->children[i].bounds, plane)) {
                        nodesToCheck.push_front(&it->children[i]);
                    }
                }
            }
        }

        return false;
    }

    bool MeshTriangle(const Mesh& mesh, const Triangle& triangle) {
        if (mesh.accelerator == nullptr) {
            // brute force our way through the triangles
            for (int i = 0; i < mesh.numTriangles; ++i) {
                if (TriangleTriangle(mesh.triangles[i], triangle)) {
                    return true;
                }
            }
            return false;
        }
        // else it has a BVH
        std::list<BVHNode*> nodesToCheck;
        nodesToCheck.push_front(mesh.accelerator);
        while (!nodesToCheck.empty()) {
            BVHNode* it = *(nodesToCheck.begin());
            nodesToCheck.erase(nodesToCheck.begin());

            // if its a leaf node
            if (it->numTriangles >= 0) {
                for (int i = 0; i < it->numTriangles; ++i) {
                    // check if it collides
                    if (TriangleTriangle(mesh.triangles[it->triangles[i]], triangle)) {
                        return true;
                    }
                }
            }

            // if its a parent node
            if (it->children != nullptr) {
                for (int i = 8 - 1; i >= 0; --i) {
                    // if it collides then add it to the list
                    if (AABBTriangle(it->children[i].bounds, triangle)) {
                        nodesToCheck.push_front(&it->children[i]);
                    }
                }
            }
        }

        return false;
    }

    void Model::SetMesh(Mesh* mesh) {
        _mesh = mesh;
        if (_mesh == nullptr) return;
        Vec3 min = mesh->vertices[0];
        Vec3 max = mesh->vertices[0];
        for (int i = 1; i < mesh->numTriangles * 3; ++i) {
            min.x = fminf(mesh->vertices[i].x, min.x);
            min.y = fminf(mesh->vertices[i].y, min.y);
            min.z = fminf(mesh->vertices[i].z, min.z);
            max.x = fmaxf(mesh->vertices[i].x, max.x);
            max.y = fmaxf(mesh->vertices[i].y, max.y);
            max.z = fmaxf(mesh->vertices[i].z, max.z);
        }
        _bounds = FromMinMax(min, max);
    }

    Mat4 GetWorldMatrix(const Model& model) {
        Mat4 translation = Translation(model.pos);
        Mat4 rotation = Rotation(model.rot.x, model.rot.y, model.rot.z);
        Mat4 localMat = rotation * translation;

        Mat4 parentMat;
        if (model.parent != nullptr) {
            parentMat = GetWorldMatrix(*model.parent);
        }

        return localMat * parentMat;
    }

    OBB GetOBB(const Model& model) {
        Mat4 world = GetWorldMatrix(model);
        const AABB& aabb = model.GetBounds();
        OBB obb;

        obb.size = aabb.size;
        obb.pos = MultiplyPoint(aabb.pos, world);
        obb.orientation = Cut(world, 3, 3);

        return obb;
    }

    float ModelRay(const Model& model, const Ray& ray) {
        // get inverse matrix
        Mat4 world = GetWorldMatrix(model);
        Mat4 inv = Inverse(world);

        // convert ray into model space
        Ray local;
        local.origin - MultiplyPoint(ray.origin, inv);
        local.dir = MultiplyVector(ray.origin, inv);
        local.NormalizeDirection();

        // ray cast in local model space
        if (model.GetMesh() != nullptr) {
            return MeshRay(*(model.GetMesh()), local);
        }
        return -1;
    }

    bool Linetest(const Model& model, const Line& line) {
        Mat4 world = GetWorldMatrix(model);
        Mat4 inv = Inverse(world);

        // convert into local space
        Line local;
        local.start = MultiplyPoint(line.start, inv);
        local.end = MultiplyPoint(line.end, inv);

        if (model.GetMesh() != nullptr) {
            return Linetest(*(model.GetMesh()), local);
        }

        return false;
    }

    bool ModelSphere(const Model& model, const Sphere& sphere) {
        Mat4 world = GetWorldMatrix(model);
        Mat4 inv = Inverse(world);

        // convert into local space
        Sphere local;
        local.pos = MultiplyPoint(sphere.pos, inv);

        if (model.GetMesh() != nullptr) {
            return MeshSphere(*(model.GetMesh()), local);
        }

        return false;
    }

    bool ModelAABB(const Model& model, const AABB& aabb) {
        Mat4 world = GetWorldMatrix(model);
        Mat4 inv = Inverse(world);

        // convert into local space and convert to obb as it could have rotations
        OBB local;
        local.size = aabb.size;
        local.pos = MultiplyPoint(aabb.pos, inv);
        local.orientation = Cut(inv, 3, 3);

        if (model.GetMesh() != nullptr) {
            return MeshOBB(*(model.GetMesh()), local);
        }
        return false;
    }

    bool ModelOBB(const Model& model, const OBB& obb) {
        Mat4 world = GetWorldMatrix(model);
        Mat4 inv = Inverse(world);

        // convert into local space
        OBB local;
        local.size = obb.size;
        local.pos = MultiplyPoint(obb.pos, inv);
        local.orientation = obb.orientation * Cut(inv, 3, 3);

        if (model.GetMesh() != nullptr) {
            return MeshOBB(*(model.GetMesh()), local);
        }
        return false;
    }

    bool ModelPlane(const Model& model, const Plane& plane) {
        Mat4 world = GetWorldMatrix(model);
        Mat4 inv = Inverse(world);

        // convert into local space
        Plane local;
        local.normal = MultiplyVector(plane.normal, inv);
        local.dist = plane.dist;

        if (model.GetMesh() != nullptr) {
            return MeshPlane(*(model.GetMesh()), local);
        }
        return false;
    }

    bool ModelTriangle(const Model& model, const Triangle& triangle) {
        Mat4 world = GetWorldMatrix(model);
        Mat4 inv = Inverse(world);

        // convert into local space
        Triangle local;
        local.a = MultiplyPoint(triangle.a, inv);
        local.b = MultiplyPoint(triangle.b, inv);
        local.c = MultiplyPoint(triangle.c, inv);

        if (model.GetMesh() != nullptr) {
            return MeshTriangle(*(model.GetMesh()), local);
        }
        return false;
    }

    Scene::Scene() : octree(nullptr) {}

    Scene::~Scene() {
        if (octree) delete octree;
    }

    bool Scene::Accelerate(const Vec3& pos, float size) {
        if (octree != nullptr) return false;

        Vec3 min(pos.x - size, pos.y - size, pos.z - size);
        Vec3 max(pos.x + size, pos.y + size, pos.z + size);

        octree = new OctreeNode();
        octree->bounds = FromMinMax(min, max);
        octree->children = nullptr;
        for (int i = 0, size = objects.size(); i < size; ++i) {
            octree->models.push_back(objects[i]);
        }
        SplitTree(octree, 5);

        return true;
    }

    void Scene::AddModel(Model* model) {
        if (std::find(objects.begin(), objects.end(), model) != objects.end()) {
            // Duplicate object, don't add
            return;
        }

        objects.push_back(model);
    }

    void Scene::RemoveModel(Model* model) {
        objects.erase(std::remove(objects.begin(), objects.end(), model), objects.end());
    }

    void Scene::UpdateModel(Model* model) {}

    std::vector<Model*> Scene::FindChildren(const Model* model) {
        std::vector<Model*> res;
        for (int i = 0, size = objects.size(); i < size; ++i) {
            if (objects[i] == nullptr || objects[i] == model) {
                continue;
            }
            Model* iterator = objects[i]->parent;
            if (iterator != nullptr) {
                if (iterator == model) {
                    res.push_back(objects[i]);
                    continue;
                }
                iterator = iterator->parent;
            }
        }

        return res;
    }

    Model* Scene::Raycast(const Ray& ray) {
        if (octree != nullptr) {
            return Physics::Raycast(octree, ray);
        }

        Model* result = nullptr;
        float result_t = -1.0f;

        for (int i = 0, size = objects.size(); i < size; ++i) {
            float t = ModelRay(*objects[i], ray);

            // on first collision set result
            if (result == nullptr && t >= 0) {
                result = objects[i];
                result_t = t;

                // store object closest to the ray origin
            } else if (result != nullptr && t < result_t) {
                result = objects[i];
                result_t = t;
            }
        }

        return result;
    }

    std::vector<Model*> Scene::Query(const Sphere& sphere) {
        if (octree != nullptr) {
            return Physics::Query(octree, sphere);
        }

        std::vector<Model*> result;
        for (int i = 0, size = objects.size(); i < size; ++i) {
            // alows for containment and intersection check
            OBB bounds = GetOBB(*objects[i]);

            if (SphereOBB(sphere, bounds)) {
                result.push_back(objects[i]);
            }
        }

        return result;
    }

    std::vector<Model*> Scene::Query(const AABB& aabb) {
        if (octree != nullptr) {
            return Physics::Query(octree, aabb);
        }
        std::vector<Model*> result;
        for (int i = 0, size = objects.size(); i < size; ++i) {
            // alows for containment and intersection check
            OBB bounds = GetOBB(*objects[i]);

            if (AABBOBB(aabb, bounds)) {
                result.push_back(objects[i]);
            }
        }

        return result;
    }

    void SplitTree(OctreeNode* node, int depth) {
        if (depth-- <= 0) {
            return;
        }

        if (node->children == nullptr) {
            // should contain 8 children
            node->children = new OctreeNode[8];

            Vec3 pos = node->bounds.pos;
            Vec3 e = node->bounds.size * 0.5f;

            // split into eight child bounding boxes. Each child bounding box shares one vertex and center
            node->children[0].bounds = AABB(pos + Vec3(-e.x, +e.y, -e.z), e);
            node->children[1].bounds = AABB(pos + Vec3(+e.x, +e.y, -e.z), e);
            node->children[2].bounds = AABB(pos + Vec3(-e.x, +e.y, +e.z), e);
            node->children[3].bounds = AABB(pos + Vec3(+e.x, +e.y, +e.z), e);
            node->children[4].bounds = AABB(pos + Vec3(-e.x, -e.y, -e.z), e);
            node->children[5].bounds = AABB(pos + Vec3(+e.x, -e.y, -e.z), e);
            node->children[6].bounds = AABB(pos + Vec3(-e.x, -e.y, +e.z), e);
            node->children[7].bounds = AABB(pos + Vec3(+e.x, -e.y, +e.z), e);
        }

        // if it has children and models re assignment models
        if (node->children != nullptr && node->models.size() > 0) {
            // for each child
            for (int i = 0; i < 8; i++) {
                for (int j = 0, size = node->models.size(); j < size; ++j) {
                    OBB bounds = GetOBB(*node->models[j]);

                    // if it intersects, add model to child
                    if (AABBOBB(node->children[i].bounds, bounds)) {
                        node->children[i].models.push_back(node->models[j]);
                    }
                }
            }
            node->models.clear();

            // recursive function for each node
            for (int i = 0; i < 8; ++i) {
                SplitTree(&(node->children[i]), depth);
            }
        }
    }

    void Insert(OctreeNode* node, Model* model) {
        OBB bounds = GetOBB(*model);
        if (AABBOBB(node->bounds, bounds)) {
            // only insert into leaf nodes
            if (node->children == nullptr) {
                node->models.push_back(model);
            } else {
                // recursively call it until u find a leaf node
                for (int i = 0; i < 8; ++i) {
                    Insert(&(node->children[i]), model);
                }
            }
        }
    }

    void Remove(OctreeNode* node, Model* model) {
        if (node->children == nullptr) {
            // find the model and remove it
            std::vector<Model*>::iterator it = std::find(node->models.begin(), node->models.end(), model);
            if (it != node->models.end()) {
                node->models.erase(it);
            }
        } else {
            // if its not a leaf node
            for (int i = 0; i < 8; ++i) {
                Remove(&(node->children[i]), model);
            }
        }
    }

    void Update(OctreeNode* node, Model* model) {
        Remove(node, model);
        Insert(node, model);
    }

    Model* FindClosest(const std::vector<Model*>& objs, const Ray& ray) {
        if (objs.size() == 0) return nullptr;

        Model* closest = nullptr;
        float closest_t = -1;

        for (size_t i = 0; i < objs.size(); i++) {
            float t = ModelRay(*objs[i], ray);

            // didnt intersect
            if (t < 0) continue;

            if (closest_t < 0 || t < closest_t) {
                closest_t = t;
                closest = objs[i];
            }
        }

        return closest;
    }

    Model* Raycast(OctreeNode* node, const Ray& ray) {
        float t = Raycast(node->bounds, ray);
        if (t < 0) return nullptr;

        if (node->children == nullptr) {
            return FindClosest(node->models, ray);
        }

        std::vector<Model*> results;
        for (int i = 0; i < 8; ++i) {
            Model* result = Raycast(&(node->children[i]), ray);
            if (result != 0) {
                results.push_back(result);
            }
        }

        return FindClosest(results, ray);
    }

    std::vector<Model*> Query(OctreeNode* node, const Sphere& sphere) {
        std::vector<Model*> result;
        if (SphereAABB(sphere, node->bounds)) {
            if (node->children == nullptr) {
                for (size_t i; i < node->models.size(); i++) {
                    OBB bounds = GetOBB(*(node->models[i]));
                    if (SphereOBB(sphere, bounds)) {
                        result.push_back(node->models[i]);
                    }
                }
            } else {
                for (size_t i; i < 8; i++) {
                    std::vector<Model*> child = Query(&(node->children[i]), sphere);
                    if (child.size() > 0) {
                        result.insert(result.end(), child.begin(), child.end());
                    }
                }
            }
        }
        return result;
    }

    std::vector<Model*> Query(OctreeNode* node, const AABB& aabb) {
        std::vector<Model*> result;
        if (AABBAABB(aabb, node->bounds)) {
            if (node->children == nullptr) {
                for (size_t i; i < node->models.size(); i++) {
                    OBB bounds = GetOBB(*(node->models[i]));
                    if (AABBOBB(aabb, bounds)) {
                        result.push_back(node->models[i]);
                    }
                }
            } else {
                for (size_t i; i < 8; i++) {
                    std::vector<Model*> child = Query(&(node->children[i]), aabb);
                    if (child.size() > 0) {
                        result.insert(result.end(), child.begin(), child.end());
                    }
                }
            }
        }
        return result;
    }

    Point Intersection(Plane p1, Plane p2, Plane p3) {
        Mat3 D(Vec3(p1.normal.x, p2.normal.x, p3.normal.x),  // Column 1
               Vec3(p1.normal.y, p2.normal.y, p3.normal.y),  // Column 2
               Vec3(p1.normal.z, p2.normal.z, p3.normal.z)   // Column 3
        );

        Vec3 A(-p1.dist, -p2.dist, -p3.dist);

        Mat3 Dx = D;
        Mat3 Dy = D;
        Mat3 Dz = D;

        Dx[0] = A;  // Row 1
        Dy[1] = A;  // Row 2
        Dz[2] = A;  // Row 3

        float detD = Determinant(D);
        if (detD == 0.0f) {
            return Point();
        }

        float detDx = Determinant(Dx);
        float detDy = Determinant(Dy);
        float detDz = Determinant(Dz);

        return Point(detDx / detD, detDy / detD, detDz / detD);
    }

    void GetCorners(const Frustum& f, Vec3* outCorners) {
        outCorners[0] = Intersection(f.near, f.top, f.left);
        outCorners[1] = Intersection(f.near, f.top, f.right);
        outCorners[2] = Intersection(f.near, f.bottom, f.left);
        outCorners[3] = Intersection(f.near, f.bottom, f.right);
        outCorners[4] = Intersection(f.far, f.top, f.left);
        outCorners[5] = Intersection(f.far, f.top, f.right);
        outCorners[6] = Intersection(f.far, f.bottom, f.left);
        outCorners[7] = Intersection(f.far, f.bottom, f.right);
    }

    bool Intersects(const Frustum& f, const Point& p) {
        for (size_t i = 0; i < 6; i++) {
            Vec3 n = f.planes[i].normal;
            float dist = f.planes[i].dist;
            float side = Dot(p, n) + dist;

            if (side < 0.0f) return false;
        }

        return true;
    }

    bool Intersects(const Frustum& f, const Sphere& s) {
        for (size_t i = 0; i < 6; i++) {
            Vec3 n = f.planes[i].normal;
            float dist = f.planes[i].dist;
            float side = Dot(s.pos, n) + dist;

            if (side < -s.radius) return false;
        }

        return true;
    }

    float Classify(const AABB& aabb, const Plane& plane) {
        float r = fabsf(aabb.size.x * plane.normal.x) + fabsf(aabb.size.y * plane.normal.y) +
                  fabsf(aabb.size.z * plane.normal.z);

        // distance from center of aabb and plane
        float d = Dot(plane.normal, aabb.pos) + plane.dist;

        // if theres no space they intersect
        if (fabsf(d) < r) {
            return 0.0f;
        } else if (d < 0.0f) {
            // else its in front
            return d + r;
        }

        // else its behind
        return d - r;
    }

    float Classify(const OBB& obb, const Plane& plane) {
        Vec3 n = MultiplyVector(plane.normal, obb.orientation);

        // max extent in dir of plane normal
        float r = fabsf(obb.size.x * n.x) + fabsf(obb.size.y * n.y) + fabsf(obb.size.z * n.z);

        // distance between box center and plane
        float d = Dot(plane.normal, obb.pos) + plane.dist;

        // if theres no space they intersect
        if (fabsf(d) < r) {
            return 0.0f;
        } else if (d < 0.0f) {
            // else its in front
            return d + r;
        }

        // else its behind
        return d - r;
    }

    bool Intersects(const Frustum& f, const AABB& aabb) {
        for (size_t i = 0; i < 6; i++) {
            if (Classify(aabb, f.planes[i]) < 0.0f) return 0;
        }

        return true;
    }

    bool Intersects(const Frustum& f, const OBB& obb) {
        for (size_t i = 0; i < 6; i++) {
            if (Classify(obb, f.planes[i]) < 0.0f) return 0;
        }

        return true;
    }

    std::vector<Model*> Scene::Cull(const Frustum& f) {
        std::vector<Model*> result;
        // if there is no octree this will be very slow
        if (octree == nullptr) {
            for (size_t i = 0; i < objects.size(); i++) {
                OBB bounds = GetOBB(*(objects[i]));
                if (Intersects(f, bounds)) {
                    result.push_back(objects[i]);
                }
            }
        } else {
            // create a list of nodes to be considered for culling
            std::list<OctreeNode*> nodes;
            nodes.push_back(octree);

            while (nodes.size() > 0) {
                OctreeNode* active = *nodes.begin();
                nodes.pop_front();

                // if its not a leaf check for intersection
                if (active->children != nullptr) {
                    for (size_t i = 0; i < 8; ++i) {
                        AABB bounds = active->children[i].bounds;

                        // if it intersects see if u can cull it further
                        if (Intersects(f, bounds)) {
                            nodes.push_back(&active->children[i]);
                        }
                    }
                } else {
                    // if its a leaf node check all objects in the node
                    for (size_t i = 0; i < active->models.size(); i++) {
                        OBB bounds = GetOBB(*(active->models[i]));
                        if (Intersects(f, bounds)) {
                            result.push_back(active->models[i]);
                        }
                    }
                }
            }
        }
        return result;
    }
}  // namespace Vakol::Physics
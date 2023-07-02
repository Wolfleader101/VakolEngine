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
        Interval b = GetInterval(obb1, axis);
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

}  // namespace Vakol::Physics
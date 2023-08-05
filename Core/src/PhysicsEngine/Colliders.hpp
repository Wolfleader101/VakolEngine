#pragma once

#include <vector>

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
    bool AABBTriangle(const AABB& aabb, const Triangle& triangle) { return TriangleAABB(triangle, aabb); }
    bool AABBPlane(const AABB& aabb, const Plane& plane);
    bool AABBSphere(const AABB& aabb, const Sphere& sphere) { return SphereAABB(sphere, aabb); }

    Interval GetInterval(const AABB& aabb, const Vec3& axis);
    Interval GetInterval(const OBB& obb, const Vec3& axis);
    bool OverlapOnAxis(const AABB& aabb, const OBB& obb, const Vec3& axis);

    //! this follows Seperate Axis Theroem (SAT) for collision
    bool AABBOBB(const AABB& aabb, const OBB& obb);

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

    Plane FromTriangle(const Triangle& t);
    Point ClosestPoint(const Triangle& t, const Point& p);

    bool TriangleSphere(const Triangle& t, const Sphere& s);

    Interval GetInterval(const Triangle& triangle, const Vec3& axis);

    bool OverlapOnAxis(const AABB& aabb, const Triangle& triangle, const Vec3& axis);
    bool TriangleAABB(const Triangle& t, const AABB& a);

    bool OverlapOnAxis(const OBB& obb, const Triangle& triangle, const Vec3& axis);
    bool TriangleOBB(const Triangle& t, const OBB& o);

    bool TrianglePlane(const Triangle& t, const Plane& p);

    bool OverlapOnAxis(const Triangle& t1, const Triangle& t2, const Vec3& axis);
    bool TriangleTriangle(const Triangle& t1, const Triangle& t2);

    // A – Edge / Triangle 0, Point 0
    // B – Edge / Triangle 0, Point 1
    // C – Edge / Triangle 1, Point 0
    // D – Edge / Triangle 1, Point 1
    Vec3 SatCrossEdge(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& d);

    Vec3 Barycentric(const Point& p, const Triangle& t);
    float Raycast(const Triangle& triangle, const Ray& ray);

    bool Linetest(const Triangle& triangle, const Line& line);

    //! warning while following the book, this uses raw pointers, ew yuck. can convert to smart pointers later
    struct BVHNode {
        AABB bounds;
        BVHNode* children;
        int numTriangles;  // if made a vector this can be removed
        int* triangles;    // triangle indicies (could be made a vector)
        BVHNode() : children(nullptr), numTriangles(0), triangles(nullptr) {}
    };

    //! warning book uses c style arrays, can use stl vector later
    struct Mesh {
        int numTriangles;
        union {
            Triangle* triangles;  // size = numTriangles
            Point* vertices;      // size = numTriangles * 3
            float* values;        // size = numTriangles * 3 * 3
        };

        BVHNode* accelerator;
        Mesh() : numTriangles(0), values(nullptr), accelerator(nullptr) {}
    };

    void AccelerateMesh(Mesh& mesh);
    void SplitBVHNode(BVHNode* node, const Mesh& model, int depth);
    void FreeBVHNode(BVHNode* node);

    float MeshRay(const Mesh& mesh, const Ray& ray);
    bool MeshAABB(const Mesh& mesh, const AABB& aabb);
    bool Linetest(const Mesh& mesh, const Line& line);
    bool MeshSphere(const Mesh& mesh, const Sphere& sphere);
    bool MeshOBB(const Mesh& mesh, const OBB& obb);
    bool MeshPlane(const Mesh& mesh, const Plane& plane);
    bool MeshTriangle(const Mesh& mesh, const Triangle& triangle);

    float Raycast(const Mesh& mesh, const Ray& ray);

    class Model {
       public:
        Vec3 pos;
        Vec3 rot;
        Model* parent;

        inline Model() : parent(nullptr), _mesh(nullptr) {}
        inline Mesh* GetMesh() const { return _mesh; }

        inline const AABB& GetBounds() const { return _bounds; }

        void SetMesh(Mesh* mesh);

       private:
        Mesh* _mesh;
        AABB _bounds;
    };

    Mat4 GetWorldMatrix(const Model& model);
    OBB GetOBB(const Model& model);

    float ModelRay(const Model& model, const Ray& ray);
    bool Linetest(const Model& model, const Line& line);
    bool ModelSphere(const Model& model, const Sphere& sphere);
    bool ModelAABB(const Model& model, const AABB& aabb);
    bool ModelOBB(const Model& model, const OBB& obb);
    bool ModelPlane(const Model& model, const Plane& plane);
    bool ModelTriangle(const Model& model, const Triangle& triangle);

    struct OctreeNode {
        AABB bounds;
        //! look into using std::array perhaps
        OctreeNode* children;
        std::vector<Model*> models;

        inline OctreeNode() : children(nullptr) {}
        inline ~OctreeNode() {
            if (children != nullptr) {
                delete[] children;
            }
        }
    };

    struct Frustum {
        union {
            struct {
                Plane top;
                Plane bottom;
                Plane left;
                Plane right;
                Plane near;
                Plane far;
            };
            Plane planes[6];
        };
        inline Frustum() {}
    };

    Point Intersection(Plane p1, Plane p2, Plane p3);
    void GetCorners(const Frustum& f, Vec3* outCorners);

    bool Intersects(const Frustum& f, const Point& p);
    bool Intersects(const Frustum& f, const Sphere& s);

    /**
     * @brief
     *
     * @param aabb aabb to test
     * @param plane plane to test
     * @return float negative if box is behind, positive if box infront, 0 if intersects
     */
    float Classify(const AABB& aabb, const Plane& plane);

    /**
     * @brief
     *
     * @param obb obb to test
     * @param plane plane to test
     * @return float negative if box is behind, positive if box infront, 0 if intersects
     */
    float Classify(const OBB& obb, const Plane& plane);
    bool Intersects(const Frustum& f, const AABB& aabb);
    bool Intersects(const Frustum& f, const OBB& obb);

    class Scene {
       public:
        Scene();
        ~Scene();

        void AddModel(Model* model);
        void RemoveModel(Model* model);
        void UpdateModel(Model* model);
        std::vector<Model*> FindChildren(const Model* model);

        Model* Raycast(const Ray& ray);
        std::vector<Model*> Query(const Sphere& sphere);
        std::vector<Model*> Query(const AABB& aabb);

        bool Accelerate(const Vec3& pos, float size);

        std::vector<Model*> Cull(const Frustum& f);

       private:
        std::vector<Model*> objects;
        OctreeNode* octree;

        Scene(const Scene&);
        Scene& operator=(const Scene&);
    };

    void SplitTree(OctreeNode* node, int depth);

    void Insert(OctreeNode* node, Model* model);
    void Remove(OctreeNode* node, Model* model);
    void Update(OctreeNode* node, Model* model);

    Model* FindClosest(const std::vector<Model*>& objs, const Ray& ray);
    Model* Raycast(OctreeNode* node, const Ray& ray);
    std::vector<Model*> Query(OctreeNode* node, const Sphere& sphere);
    std::vector<Model*> Query(OctreeNode* node, const AABB& aabb);

}  // namespace Vakol::Physics
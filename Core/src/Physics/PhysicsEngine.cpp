#include "Physics/PhysicsEngine.hpp"

#include "Logger/Logger.hpp"
#include "Physics/PhysicsScene.hpp"

#include <algorithm>

namespace Vakol
{
    constexpr Math::Vec3 GRAVITY = Math::Vec3(0.0f, -9.81f, 0.0f);

    PhysicsEngine::PhysicsEngine()
    {
    }

    PhysicsScene& PhysicsEngine::CreateScene()
    {
        rp3d::PhysicsWorld* newWorld = m_rpCommon.createPhysicsWorld();
        PhysicsScene newScene(newWorld);

        std::unique_ptr<Vakol::PhysicsScene> newScenePtr = std::make_unique<PhysicsScene>(newScene);
        m_scenes.push_back(std::move(newScenePtr));

        return *m_scenes.back();
    }

    void PhysicsEngine::DestroyScene(PhysicsScene& scene)
    {
        auto& targetGuid = scene.GetGuid();

        auto it = std::find_if(
            m_scenes.begin(), m_scenes.end(),
            [&targetGuid](const std::unique_ptr<Vakol::PhysicsScene>& s) { return s->m_guid == targetGuid; });

        if (it != m_scenes.end())
        {
            m_rpCommon.destroyPhysicsWorld(it->get()->m_world);
            m_scenes.erase(it);
        }
    }

    BoxCollider PhysicsEngine::CreateBoxCollider(Math::Vec3& halfExtents)
    {
        BoxCollider collider;

        collider.shape = m_rpCommon.createBoxShape(rp3d::Vector3(halfExtents.x, halfExtents.y, halfExtents.z));
        collider.collider = nullptr;

        return collider;
    }

    SphereCollider PhysicsEngine::CreateSphereCollider(const float radius)
    {
        SphereCollider collider;

        collider.shape = m_rpCommon.createSphereShape(radius);
        collider.collider = nullptr;

        return collider;
    }

    CapsuleCollider PhysicsEngine::CreateCapsuleCollider(const float radius, const float height)
    {
        CapsuleCollider collider;

        collider.shape = m_rpCommon.createCapsuleShape(radius, height);
        collider.collider = nullptr;

        return collider;
    }

    MeshCollider PhysicsEngine::CreateMeshCollider(std::vector<Math::Point>& vertices,
                                                   std::vector<unsigned int>& indices)
    {
        MeshCollider collider;

        // rp3d::TriangleVertexArray* triangleArray = new rp3d::TriangleVertexArray(
        //     vertices.size(),          // number of vertices
        //     vertices.data(),          // start of vertex data
        //     sizeof(Math::Point),      // stride of vertex data (3 floats per vertex)
        //     indices.size() / 3,       // number of triangles (assuming 3 indices per triangle)
        //     indices.data(),           // start of index data
        //     sizeof(unsigned int) * 3, // stride of index data (3 indices per triangle)
        //     rp3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE, // vertex data type
        //     rp3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE  // index data type
        //);

        // rp3d::TriangleMesh* triangleMesh = m_rpCommon.createTriangleMesh();

        // triangleMesh->addSubpart(triangleArray);

        // collider.shape = m_rpCommon.createConcaveMeshShape(triangleMesh);
        collider.collider = nullptr;

        VK_WARN("Mesh Collider not supported!");

        return collider;
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, BoxCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());
        rb.inertiaTensor = CalculateInertiaTensor(rb.mass, collider);
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, SphereCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());
        rb.inertiaTensor = CalculateInertiaTensor(rb.mass, collider);
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, CapsuleCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, MeshCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());
    }

    Math::Mat3 PhysicsEngine::CalculateInertiaTensor(const float mass, const BoxCollider& collider)
    {
        Math::Vec3 i = Math::Vec3(0.0f);

        constexpr float K = 1.0f / 12.0f;

        // get extents by multiplying the half-extents by 2
        Math::Vec3 extents = FromRPVec3(collider.shape->getHalfExtents()) * 2.0f;

        // extents squared
        extents *= extents;

        i.x = (extents.y + extents.z) * mass * K;
        i.y = (extents.x + extents.z) * mass * K;
        i.z = (extents.x + extents.y) * mass * K;

        return {i.x, 0.0f, 0.0f, 0.0f, i.y, 0.0f, 0.0f, 0.0f, i.z};
    }

    Math::Mat3 PhysicsEngine::CalculateInertiaTensor(const float mass, const SphereCollider& collider)
    {
        Math::Vec3 i = Math::Vec3(0.0f);

        constexpr float K = 2.0f / 5.0f;

        // radius squared
        float r2 = collider.shape->getRadius() * collider.shape->getRadius();

        i.x = r2 * mass * K;
        i.y = r2 * mass * K;
        i.z = r2 * mass * K;

        return {i.x, 0.0f, 0.0f, 0.0f, i.y, 0.0f, 0.0f, 0.0f, i.z};
    }

    void PhysicsEngine::ApplyForces(Math::Vec3& pos, Math::Quat& rot, RigidBody& rb)
    {
        rb.collisionData->parentBody = &rb;

        // Don't apply forces to static bodies
        if (rb.type == BodyType::Static)
        {
            rb.collisionBody->setTransform(
                rp3d::Transform(ToRPVec3(pos), rp3d::Quaternion(rot.x, rot.y, rot.z, rot.w)));

            return;
        }

        // F = ma
        Math::Vec3 force = rb.mass * GRAVITY;

        // a = F / m
        Math::Vec3 linearAcceleration = force / rb.mass;

        // Vf = Vi + a(t)
        rb.linearVelocity += linearAcceleration * static_cast<float>(m_timeStep);

        pos += rb.linearVelocity * static_cast<float>(m_timeStep);

        rb.collisionBody->setTransform(rp3d::Transform(ToRPVec3(pos), rp3d::Quaternion(rot.x, rot.y, rot.z, rot.w)));
    }

    void PhysicsEngine::DetectCollisions(PhysicsScene& scene)
    {
        // Let reactphysics3d handle the collision detection
        scene.Update(m_timeStep);
    }

    void PhysicsEngine::ResolveCollisions(Math::Vec3& pos, RigidBody& rb)
    {
        if (!rb.collisionData->isColliding || rb.type == BodyType::Static)
            return;

        const Math::Vec3 normal = Math::Normalized(rb.collisionData->worldContactNormal);
        const float relativeNormalVelocity = Math::Dot(rb.collisionData->relativeVelocity, normal);

        if (relativeNormalVelocity > 0.0f)
            return;

        const float impulseMagnitude = -(1.0f + rb.bounciness) * relativeNormalVelocity;

        const Math::Vec3 impulse = impulseMagnitude * -normal;
        const Math::Vec3 depenetration = rb.collisionData->penetrationDepth * -normal;

        rb.linearVelocity += impulse / rb.mass + depenetration;
    }

    void PhysicsEngine::Depenetration(Math::Vec3& pos, RigidBody& rb)
    {
    }

    void PhysicsEngine::SetTimeStep(double step)
    {
        m_timeStep = step;
    }

    double PhysicsEngine::GetTimeStep() const
    {
        return m_timeStep;
    }

} // namespace Vakol
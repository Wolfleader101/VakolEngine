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
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, SphereCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, CapsuleCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, MeshCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());
    }

    void PhysicsEngine::ApplyForces(Math::Vec3& pos, Math::Quat& rot, RigidBody& rb)
    {
        if (rb.type == BodyType::Static)
        {
            rb.collisionBody->setTransform(rp3d::Transform(
                rp3d::Vector3(pos.x, pos.y, pos.z), rp3d::Quaternion(rot.x, rot.y, rot.z, rot.w)));

            return;
        }

        rb.linearVelocity = rb.linearVelocity + GRAVITY * static_cast<float>(m_timeStep);

        pos = pos + rb.linearVelocity * static_cast<float>(m_timeStep);

        rb.collisionBody->setTransform(rp3d::Transform(ToRPVec3(pos), rp3d::Quaternion(rot.x, rot.y, rot.z, rot.w)));
    }

    void PhysicsEngine::DetectCollisions(PhysicsScene& scene)
    {
        scene.Update(m_timeStep);
    }

    void PhysicsEngine::ResolveCollisions(Math::Vec3& pos, RigidBody& rb)
    {
        // Exit if there's no collision data or if the body is static
        if (!rb.collisionData || rb.type == BodyType::Static)
            return;

        if (!rb.collisionData->isColliding)
            return;
    }

    void PhysicsEngine::Depenetration(Math::Vec3& pos, RigidBody& rb)
    {
        if (!rb.collisionData || rb.type == BodyType::Static)
            return;

        if (!rb.collisionData->isColliding)
            return;

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
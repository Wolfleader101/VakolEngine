#include "Physics/PhysicsEngine.hpp"
#include "Physics/PhysicsScene.hpp"

#include <algorithm>

namespace Vakol
{
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
            [&targetGuid](const std::unique_ptr<Vakol::PhysicsScene>& s) { return s->GetGuid() == targetGuid; });

        if (it != m_scenes.end())
        {
            m_rpCommon.destroyPhysicsWorld(it->get()->m_world);
            m_scenes.erase(it);
        }
    }

    AABBCollider PhysicsEngine::CreateAABBCollider(Math::Vec3& halfExtents)
    {
        AABBCollider collider;
        collider.shape = m_rpCommon.createBoxShape(
            rp3d::Vector3((double)halfExtents.x, (double)halfExtents.y, (double)halfExtents.z));
        collider.collider = nullptr;
        return collider;
    }

    SphereCollider PhysicsEngine::CreateSphereCollider(double radius)
    {
        SphereCollider collider;
        collider.shape = m_rpCommon.createSphereShape(radius);
        collider.collider = nullptr;
        return collider;
    }

    CapsuleCollider PhysicsEngine::CreateCapsuleCollider(double radius, double height)
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

        rp3d::TriangleVertexArray* triangleArray = new rp3d::TriangleVertexArray(
            vertices.size(),      // number of vertices
            vertices.data(),      // start of vertex data
            sizeof(Math::Point),  // stride of vertex data
            indices.size() / 3ul, // number of triangles, assuming 3 indices per triangle
            indices.data(),       // start of index data
            sizeof(unsigned int), // stride of index data
            rp3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE, // vertex data type
            rp3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE  // index data type
        );

        rp3d::TriangleMesh* triangleMesh = m_rpCommon.createTriangleMesh();

        triangleMesh->addSubpart(triangleArray);

        collider.shape = m_rpCommon.createConcaveMeshShape(triangleMesh);
        collider.collider = nullptr;

        return collider;
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, AABBCollider& collider)
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

    void PhysicsEngine::ApplyForces(RigidBody& rb)
    {
        if (rb.type == BodyType::Static)
        {
            return;
        }

        // linear velocity: V = u + at

        Math::Vec3 gravity(0.0f, -9.8f, 0.0f);

        Math::Vec3 dv = gravity * (float)m_timeStep;

        rb.linearVelocity += dv;

        // Get current position from transform

        Math::Vec3 currentPosition(rb.collisionBody->getTransform().getPosition().x,
                                   rb.collisionBody->getTransform().getPosition().y,
                                   rb.collisionBody->getTransform().getPosition().z);

        // Compute new position based on velocity and time step
        Math::Vec3 newPosition = currentPosition + rb.linearVelocity * (float)m_timeStep;

        // Update transform with new position
        rb.collisionBody->setTransform(
            rp3d::Transform(rp3d::Vector3((float)newPosition.x, (float)newPosition.y, (float)newPosition.z),
                            rb.collisionBody->getTransform().getOrientation()));
    }

    void PhysicsEngine::DetectCollisions(PhysicsScene& scene)
    {
        scene.Update(m_timeStep);
    }

    void PhysicsEngine::ResolveCollisions(RigidBody& rb)
    {
        // Exit if there's no collision data or if the body is static
        if (!rb.collisionData || rb.type == BodyType::Static)
        {
            return;
        }

        Math::Vec3 collisionNormal = rb.collisionData->normal;

        // Calculate the velocity component along the collision normal
        float velAlongNormal = Math::Dot(rb.linearVelocity, collisionNormal);

        // If the velocity is away from the collision, do nothing
        if (velAlongNormal > 0)
        {
            return;
        }

        // Add a restitution factor between 0 and 1
        float restitution = 0.7f;

        // Calculate the new reflected velocity
        Math::Vec3 reflectedVelocity = rb.linearVelocity - (1.0f + restitution) * velAlongNormal * collisionNormal;

        // Update the body's velocity
        rb.linearVelocity = reflectedVelocity;

        // Small distance to move along the collision normal to resolve overlap

        // Calculate the new position based on the collision normal and small distance
        Math::Vec3 currentPosition(rb.collisionBody->getTransform().getPosition().x,
                                   rb.collisionBody->getTransform().getPosition().y,
                                   rb.collisionBody->getTransform().getPosition().z);

        Math::Vec3 newPosition = currentPosition + (float)rb.collisionData->penetrationDepth * collisionNormal;

        // Update the transform with the new position
        rb.collisionBody->setTransform(
            rp3d::Transform(rp3d::Vector3((float)newPosition.x, (float)newPosition.y, (float)newPosition.z),
                            rb.collisionBody->getTransform().getOrientation()));

        rb.collisionData->penetrationDepth = 0.0f;
        rb.collisionData->normal = Math::Vec3(0.0f, 0.0f, 0.0f);
        rb.collisionData->worldPoint = Math::Vec3(0.0f, 0.0f, 0.0f);
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
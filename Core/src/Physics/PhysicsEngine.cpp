#include "Physics/PhysicsEngine.hpp"
#include "Physics/PhysicsScene.hpp"

#include <algorithm>

namespace Vakol
{
    Math::Vec3 FromRPVec3(rp3d::Vector3& vec)
    {
        return Math::Vec3((float)vec.x, (float)vec.y, (float)vec.z);
    }

    Math::Vec3 FromRPVec3(const rp3d::Vector3& vec)
    {
        return Math::Vec3((float)vec.x, (float)vec.y, (float)vec.z);
    }

    rp3d::Vector3 ToRPVec3(Math::Vec3& vec)
    {
        return rp3d::Vector3((double)vec.x, (double)vec.y, (double)vec.z);
    }

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

        rb.centerOfMass = FromRPVec3(collider.collider->getLocalToBodyTransform().getPosition());
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, SphereCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());

        rb.centerOfMass = FromRPVec3(collider.collider->getLocalToBodyTransform().getPosition());
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
            //! THIS IS A HACK TO MAKE SURE THAT THE COLLIDER FOLLOWS THE TRANSFORM
            rb.collisionBody->setTransform(rp3d::Transform(
                rp3d::Vector3(static_cast<double>(pos.x), static_cast<double>(pos.y), static_cast<double>(pos.z)),
                rp3d::Quaternion(static_cast<double>(rot.x), static_cast<double>(rot.y), static_cast<double>(rot.z),
                                 static_cast<double>(rot.w))));
            return;
        }

        // can be assumed as static, can be moved later
        static Math::Vec3 gravity(0.0f, -9.8f, 0.0f);

        rb.force += rb.mass * gravity;

        rb.linearVelocity += rb.force * static_cast<float>(m_timeStep);

        pos += rb.linearVelocity * static_cast<float>(m_timeStep);

        // Update transform with new position
        rb.collisionBody->setTransform(rp3d::Transform(
            rp3d::Vector3(static_cast<double>(pos.x), static_cast<double>(pos.y), static_cast<double>(pos.z)),
            rp3d::Quaternion(static_cast<double>(rot.x), static_cast<double>(rot.y), static_cast<double>(rot.z),
                             static_cast<double>(rot.w))));

        // reset the force
        rb.force = Math::Vec3(0.0f, 0.0f, 0.0f);
    }

    void PhysicsEngine::DetectCollisions(PhysicsScene& scene)
    {
        scene.Update(m_timeStep);
    }

    void PhysicsEngine::ResolveCollisions(Math::Vec3& pos, Math::Quat& rot, RigidBody& rb)
    {
        // Exit if there's no collision data or if the body is static
        if (!rb.collisionData || rb.type == BodyType::Static)
        {
            return;
        }

        if (!rb.collisionData->isColliding)
            return;

        // reflect the velocity
        rb.linearVelocity = rb.linearVelocity - 2 * Math::Dot(rb.linearVelocity, rb.collisionData->worldNormal) *
                                                    rb.collisionData->worldNormal;

        // multiply velocity by coefficient of restitution
        rb.linearVelocity *= rb.bounciness;

        Math::Vec3 depenetration =
            -rb.collisionData->worldNormal * static_cast<float>(rb.collisionData->penetrationDepth);
        pos = pos + depenetration;

        // another valid de-penetration is get the difference in position between last frame and this frame using the
        // velocity, and move it back by that amount

        rb.collisionBody->setTransform(rp3d::Transform(
            rp3d::Vector3(static_cast<double>(pos.x), static_cast<double>(pos.y), static_cast<double>(pos.z)),
            rp3d::Quaternion(static_cast<double>(rot.x), static_cast<double>(rot.y), static_cast<double>(rot.z),
                             static_cast<double>(rot.w))));

        // reset the collision data
        rb.collisionData->penetrationDepth = 0.0;
        rb.collisionData->worldNormal = Math::Vec3(0.0f, 0.0f, 0.0f);
        rb.collisionData->worldPoint = Math::Vec3(0.0f, 0.0f, 0.0f);
        rb.collisionData->isColliding = false;
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
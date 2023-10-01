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
        rb.localInertiaTensor = CalculateInertiaTensor(rb.mass, collider);
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, SphereCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());
        rb.localInertiaTensor = CalculateInertiaTensor(rb.mass, collider);
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
        rb.contactData->parentBody = &rb;

        rb.centreOfMass = pos;
        rb.rotationMatrix = Math::Mat3Cast(rot);
        rb.worldInertiaTensor = rb.rotationMatrix * rb.localInertiaTensor * Math::Transpose(rb.rotationMatrix);

        // Don't apply forces to static bodies
        if (rb.type == BodyType::Static)
        {
            rb.collisionBody->setTransform(
                rp3d::Transform(ToRPVec3(pos), rp3d::Quaternion(rot.x, rot.y, rot.z, rot.w)));

            return;
        }

        // F = ma
        rb.force += rb.mass * GRAVITY;

        // a = F / m
        const Math::Vec3 linearAcceleration = rb.force / rb.mass;

        // α = Iτ
        const Math::Vec3 angularAcceleration = rb.worldInertiaTensor * rb.torque;

        // Vf = Vi + a(t)
        rb.linearVelocity += linearAcceleration * static_cast<float>(m_timeStep);
        rb.angularVelocity += angularAcceleration * static_cast<float>(m_timeStep);

        pos += rb.linearVelocity * static_cast<float>(m_timeStep);
        rot = Math::Normalized(rot + Math::Quat(0.0f, rb.angularVelocity * static_cast<float>(m_timeStep)) * rot);

        rb.collisionBody->setTransform(rp3d::Transform(ToRPVec3(pos), rp3d::Quaternion(rot.x, rot.y, rot.z, rot.w)));

        // Reset forces and torques
        rb.force = Math::Vec3(0.0f);
        rb.torque = Math::Vec3(0.0f);
    }

    void PhysicsEngine::DetectCollisions(PhysicsScene& scene)
    {
        // Let reactphysics3d handle the collision detection
        scene.Update(m_timeStep);
    }

    Math::Vec3 PhysicsEngine::SolveImpulse(const ContactPair& contactPair)
    {
        const Math::Vec3 contactNormal = Math::Normalized(contactPair.worldContactNormal);

        // (v2 - v1) . n (relative velocity along normal)
        const float rNV = Math::Dot(contactPair.relativeVelocity, contactNormal);

        // (r1 x n) (relative local contact point 1 along normal)
        const Math::Vec3 r1N = Math::Cross(contactPair.contactA->relativeLocalContactPoint, contactNormal);

        // (r2 x n) (relative local contact point 2 along normal)
        const Math::Vec3 r2N = Math::Cross(contactPair.contactB->relativeLocalContactPoint, contactNormal);

        // minimum coefficient of restitution
        const float e = fminf(contactPair.contactA->parentBody->bounciness, contactPair.contactB->parentBody->bounciness);

        const float numerator = -(1.0f + e) * rNV;

        // first part of the denominator
        const float d1 = 2.0f; // inverse mass sum (assume both masses are 1)

        // second part of the denominator
        const Math::Vec3 d2 = Math::Cross(r1N * Math::Inverse(contactPair.contactA->parentBody->worldInertiaTensor),
                                    contactPair.contactA->relativeLocalContactPoint);

        // third part of the denominator
        const Math::Vec3 d3 = Math::Cross(r2N * Math::Inverse(contactPair.contactB->parentBody->worldInertiaTensor),
                                    contactPair.contactB->relativeLocalContactPoint);

        const float denominator = d1 + Math::Dot(contactNormal, d2 + d3);

        float lambda = (denominator == 0.0f) ? 0.0f : numerator / denominator;

        if (contactPair.contactCount > 0 && lambda != 0.0f)
            lambda /= static_cast<float>(contactPair.contactCount);

        return contactNormal * lambda;
    }

    void PhysicsEngine::ResolveCollisions(Math::Vec3& pos, RigidBody& rb)
    {
        if (!rb.contactData || rb.type == BodyType::Static)
            return;
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
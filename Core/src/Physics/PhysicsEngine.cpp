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
        rb.localInertiaTensor = CalculateInertiaTensor(rb.mass, FromRPVec3(collider.shape->getHalfExtents()));
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, SphereCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());
        rb.localInertiaTensor = CalculateInertiaTensor(rb.mass, collider.shape->getRadius());
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, CapsuleCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, MeshCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());
    }

    Math::Mat3 PhysicsEngine::CalculateInertiaTensor(const float mass, const Math::Vec3& halfExtents)
    {
        Math::Vec3 i = Math::Vec3(0.0f);

        constexpr float K = 1.0f / 12.0f;

        // get extents by multiplying the half-extents by 2
        Math::Vec3 extents = halfExtents;

        // extents squared
        extents *= extents;

        i.x = K * mass * (extents.y + extents.z);
        i.y = K * mass * (extents.x + extents.z);
        i.z = K * mass * (extents.x + extents.y);

        return {i.x, 0.0f, 0.0f, 0.0f, i.y, 0.0f, 0.0f, 0.0f, i.z};
    }

    Math::Mat3 PhysicsEngine::CalculateInertiaTensor(const float mass, const float radius)
    {
        Math::Vec3 i = Math::Vec3(0.0f);

        constexpr float K = 2.0f / 5.0f;

        // radius squared
        float r2 = radius * radius;

        i.x = r2 * mass * K;
        i.y = r2 * mass * K;
        i.z = r2 * mass * K;

        return {i.x, 0.0f, 0.0f, 0.0f, i.y, 0.0f, 0.0f, 0.0f, i.z};
    }

    void PhysicsEngine::ApplyForces(Math::Vec3& pos, Math::Quat& rot, RigidBody& rb)
    {
        rb.contactData->parentBody = &rb;

        // Don't apply forces to static bodies
        if (rb.type == BodyType::Static)
        {
            rb.collisionBody->setTransform(
                rp3d::Transform(ToRPVec3(pos), rp3d::Quaternion(rot.x, rot.y, rot.z, rot.w)));

            return;
        }

        // F = ma
        if (rb.useGravity)
            rb.force += rb.mass * GRAVITY;

        // a = F / m
        const Math::Vec3 linearAcceleration = rb.force / rb.mass;

        // α = Iτ
        const Math::Vec3 angularAcceleration = rb.localInertiaTensor * rb.torque;

        // Vf = Vi + a(t)
        rb.linearVelocity += linearAcceleration * static_cast<float>(m_timeStep);
        rb.angularVelocity += angularAcceleration * static_cast<float>(m_timeStep);

        pos += rb.linearVelocity * static_cast<float>(m_timeStep);
        rot = Math::Normalized(rot + Math::Quat(0.0f, rb.angularVelocity * static_cast<float>(m_timeStep) * 0.5f) * rot);

        rb.collisionBody->setTransform(rp3d::Transform(ToRPVec3(pos), rp3d::Quaternion(rot.x, rot.y, rot.z, rot.w)));

        rb.centreOfMass = pos;
        rb.rotationMatrix = Math::Mat3Cast(rot);

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
        auto& bodyA = contactPair.contactA;
        auto& bodyB = contactPair.contactB;

        const Math::Vec3 n = Math::Normalized(contactPair.worldContactNormal);

        // relative velocity along normal
        const float rVN = Math::Dot(contactPair.relativeVelocity, n);

        // epsilon coefficient of restitution
        const float e = fminf(bodyA->parentBody->bounciness, bodyB->parentBody->bounciness);

        // numerator of lambda (impulse magnitude)
        const float numerator = -(1.0f + e) * rVN;

        // first part of the denominator of lambda
        const float d1a = 1.0f / bodyA->parentBody->mass;
        const float d1b = 1.0f / bodyB->parentBody->mass;
        const float d1 = d1a + d1b;

        // second part of the denominator of lambda
        const Math::Vec3 d2a = Math::Inverse(bodyA->parentBody->localInertiaTensor) * Math::Cross(Math::Cross(bodyA->relativeLocalContactPoint, n), bodyA->relativeLocalContactPoint);
        const Math::Vec3 d2b = Math::Inverse(bodyB->parentBody->localInertiaTensor) * Math::Cross(Math::Cross(bodyB->relativeLocalContactPoint, n), bodyB->relativeLocalContactPoint);

        // denominator of lambda
        const float denominator = d1 + Math::Dot(d2a + d2b, n);

        const float lambda = numerator / denominator;

        return lambda * n;
    }

    void PhysicsEngine::ResolveCollisions(Math::Vec3& pos, RigidBody& rb)
    {
        if (!rb.contactData || !rb.contactData->contactPair || !rb.contactData->contactPair->isColliding)
            return;

        const auto& contactPair = rb.contactData->contactPair;
        const auto& contactA = contactPair->contactA;
        const auto& contactB = contactPair->contactB;

        const Math::Vec3 depenatration = -contactPair->worldContactNormal * contactPair->penetrationDepth;

        rb.linearVelocity += contactPair->impulseSum / rb.mass + depenatration;
        rb.angularVelocity += Math::Cross(contactA->relativeLocalContactPoint, contactPair->impulseSum) * Math::Inverse(rb.localInertiaTensor);

        //contactA->parentBody->linearVelocity += contactPair->impulseSum / contactA->parentBody->mass + depenatration;
        //contactA->parentBody->angularVelocity += Math::Cross(contactPair->contactA->relativeLocalContactPoint, contactPair->impulseSum) * Math::Inverse(contactA->parentBody->localInertiaTensor);

        //contactB->parentBody->linearVelocity -= contactPair->impulseSum / contactB->parentBody->mass + depenatration;
        //contactB->parentBody->angularVelocity -= Math::Cross(contactPair->contactB->relativeLocalContactPoint, contactPair->impulseSum) * Math::Inverse(contactB->parentBody->localInertiaTensor);
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
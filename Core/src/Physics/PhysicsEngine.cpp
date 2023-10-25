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
        //rb.collider = collider.collider;

        auto inertiaTensor = FromRPMat3(collider.shape->getLocalInertiaTensor(rb.mass));
        auto invIT = Math::Inverse(inertiaTensor);

        rb.inertiaTensor = {inertiaTensor[0][0], inertiaTensor[1][1], inertiaTensor[2][2]};
        rb.inverseInertiaTensor = {invIT[0][0], invIT[1][1], invIT[2][2]};

        rb.mass = rb.type == BodyType::Static ? 0.0f : rb.mass;
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, SphereCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());
        //rb.collider = collider.collider;

        auto inertiaTensor = FromRPMat3(collider.shape->getLocalInertiaTensor(rb.mass));
        auto invIT = Math::Inverse(inertiaTensor);

        rb.inertiaTensor = {inertiaTensor[0][0], inertiaTensor[1][1], inertiaTensor[2][2]};
        rb.inverseInertiaTensor = {invIT[0][0], invIT[1][1], invIT[2][2]};

        rb.mass = rb.type == BodyType::Static ? 0.0f : rb.mass;
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, CapsuleCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());
        //rb.collider = collider.collider;

        auto inertiaTensor = FromRPMat3(collider.shape->getLocalInertiaTensor(rb.mass));
        auto invIT = Math::Inverse(inertiaTensor);

        rb.inertiaTensor = {inertiaTensor[0][0], inertiaTensor[1][1], inertiaTensor[2][2]};
        rb.inverseInertiaTensor = {invIT[0][0], invIT[1][1], invIT[2][2]};

        rb.mass = rb.type == BodyType::Static ? 0.0f : rb.mass;
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, MeshCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());
        //rb.collider = collider.collider;

        auto inertiaTensor = FromRPMat3(collider.shape->getLocalInertiaTensor(rb.mass));
        auto invIT = Math::Inverse(inertiaTensor);

        rb.inertiaTensor = {inertiaTensor[0][0], inertiaTensor[1][1], inertiaTensor[2][2]};
        rb.inverseInertiaTensor = {invIT[0][0], invIT[1][1], invIT[2][2]};

        rb.mass = rb.type == BodyType::Static ? 0.0f : rb.mass;
    }

    Math::Mat3 PhysicsEngine::CalculateInertiaTensor(const float mass, const Math::Vec3& halfExtents)
    {
        Math::Vec3 i = Math::Vec3(0.0f);

        const float K = 1.0f / 12.0f * mass;

        Math::Vec3 extents = halfExtents;

        // extents squared
        extents *= extents;

        i.x = K * (extents.y + extents.z);
        i.y = K * (extents.x + extents.z);
        i.z = K * (extents.x + extents.y);

        return {i.x, 0.0f, 0.0f, 0.0f, i.y, 0.0f, 0.0f, 0.0f, i.z};
    }

    Math::Mat3 PhysicsEngine::CalculateInertiaTensor(const float mass, const float radius)
    {
        Math::Vec3 i = Math::Vec3(0.0f);

        const float K = 2.0f / 5.0f * mass;

        // radius squared
        const float r2 = radius * radius;

        i.x = K * r2;
        i.y = K * r2;
        i.z = K * r2;

        return {i.x, 0.0f, 0.0f, 0.0f, i.y, 0.0f, 0.0f, 0.0f, i.z};
    }

    void PhysicsEngine::ApplyConstraints(RigidBody& rb)
    {
        if (rb.constraints[0][0] == 1)
            rb.linearVelocity.x = 0.0f;
        if (rb.constraints[0][1] == 1)
            rb.linearVelocity.y = 0.0f;
        if (rb.constraints[0][2] == 1)
            rb.linearVelocity.z = 0.0f;

        if (rb.constraints[1][0] == 1)
            rb.angularVelocity.x = 0.0f;
        if (rb.constraints[1][1] == 1)
            rb.angularVelocity.y = 0.0f;
        if (rb.constraints[1][2] == 1)
            rb.angularVelocity.z = 0.0f;
    }

    void PhysicsEngine::ApplyForces(Math::Vec3& pos, Math::Quat& rot, RigidBody& rb)
    {
        rb.contactData->parentBody = &rb;

        // Don't apply forces to static bodies
        if (rb.type == BodyType::Static)
        {
            rb.SetTransform(pos, rot);

            return;
        }

        // F = ma
        if (rb.useGravity)
            rb.force += rb.mass * GRAVITY;

        // a = F / m
        const Math::Vec3 linearAcceleration = rb.force * rb.GetInverseMass();

        // α = Iτ
        const Math::Vec3 angularAcceleration = rb.inverseInertiaTensor * rb.torque;

        // Vf = Vi + a(t)
        rb.linearVelocity += linearAcceleration * static_cast<float>(m_timeStep);

        // w = α(t)
        rb.angularVelocity += angularAcceleration * static_cast<float>(m_timeStep) * 0.98f;

        ApplyConstraints(rb);

        pos += rb.linearVelocity * static_cast<float>(m_timeStep);

        rot = Math::Normalized(rot + Math::Quat(0.0f, rb.angularVelocity * static_cast<float>(m_timeStep) * 0.5f) * rot);

        rb.SetTransform(pos, rot);

        // Reset forces and torques
        rb.ResetForces();
        rb.ResetTorques();
    }

    void PhysicsEngine::DetectCollisions(PhysicsScene& scene)
    {
        // Let reactphysics3d handle the collision detection
        scene.Update(m_timeStep);
    }

    void PhysicsEngine::ResolveCollisions(RigidBody& bodyA, RigidBody& bodyB, const Math::Vec3& normal, const Math::Vec3& cPointA, const Math::Vec3& cPointB)
    {
        const Math::Vec3 v1 = bodyA.linearVelocity;
        const Math::Vec3 v2 = bodyB.linearVelocity;

        const Math::Vec3 w1 = bodyA.angularVelocity;
        const Math::Vec3 w2 = bodyB.angularVelocity;

        const Math::Vec3 J1 = bodyA.inverseInertiaTensor;
        const Math::Vec3 J2 = bodyB.inverseInertiaTensor;

        const Math::Vec3 r1 = cPointA - bodyA.centreOfMass;
        const Math::Vec3 r2 = cPointB - bodyB.centreOfMass;

        const Math::Vec3 rVelA = v1 + Math::Cross(w1, r1);
        const Math::Vec3 rVelB = v2 + Math::Cross(w2, r2);

        const Math::Vec3 r1xN = Math::Cross(r1, normal);
        const Math::Vec3 r2xN = Math::Cross(r2, normal);

        // clamp between 0 and 1 to avoid unexpected results
        const float e = std::clamp((bodyA.bounciness + bodyB.bounciness) / 2.0f, 0.0f, 1.0f);
        const float n1 = -(1.0f + e);

        const float n2 = Math::Dot(normal, v1 - v2);

        const float n3a = Math::Dot(w1, r1xN);
        const float n3b = Math::Dot(w2, r2xN);
        const float n3 = n3a - n3b;

        const float numerator = n1 * (n2 + n3);

        //inverse mass sum
        const float mSum = bodyA.GetInverseMass() + bodyB.GetInverseMass();

        const float d2a = Math::Dot(r1xN, J1 * r1xN);
        const float d2b = Math::Dot(r2xN, J2 * r2xN);
        const float d2 = d2a + d2b;

        const float denominator = mSum + d2;

        const float lambda = numerator / denominator;

        // bodies are separating; no collision impulse is needed.
        if (lambda >= 0.0f)
            return;

        const Math::Vec3 impulse = lambda * normal;

        if (bodyA.type != BodyType::Static)
        {
            bodyA.linearVelocity = v1 + impulse * bodyA.GetInverseMass();
            bodyA.angularVelocity = w1 + lambda * bodyA.inverseInertiaTensor * r1xN;
        }

        if (bodyB.type != BodyType::Static)
        {
            bodyB.linearVelocity = v2 - impulse * bodyB.GetInverseMass();
            bodyB.angularVelocity = w2 - lambda * bodyB.inverseInertiaTensor * r2xN;
        }
    }

    // Derived from Game Physics Cookbook http://ndl.ethernet.edu.et/bitstream/123456789/24446/1/Gabor%20Szauer_2017.pdf
    void PhysicsEngine::Depenetration(RigidBody& bodyA, RigidBody& bodyB, const float penetrationDepth, const Math::Vec3& normal)
    {
        if (bodyA.type == BodyType::Static && bodyB.type == BodyType::Static)
            return;

        
        const float invMassSum = bodyA.GetInverseMass() + bodyB.GetInverseMass();

        if (invMassSum == 0.0f)
            return;

        // How much positional correction to apply. Smaller values will allow more penetration to occur.
        constexpr float linearProjectionPercent = 0.45f;

        // Determines how much to allow objects to penetrate.
        constexpr float penetrationSlack = 0.01f; // Keep value between 0.01 and 0.1

        const float depth = fmaxf(penetrationDepth - penetrationSlack, 0.0f);
        const float scalar = depth / invMassSum;

        const Math::Vec3 correction = normal * scalar * linearProjectionPercent;

        if (bodyA.type != BodyType::Static)
        {
            bodyA.position -= correction * bodyA.GetInverseMass();
            bodyA.SetPosition(bodyA.position);
        }

        if (bodyB.type != BodyType::Static)
        {
            bodyB.position += correction * bodyB.GetInverseMass();
            bodyB.SetPosition(bodyB.position);
        }
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
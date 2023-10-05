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
        rb.localInverseInertiaTensor = Math::Inverse(rb.localInertiaTensor);
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, SphereCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());

        rb.localInertiaTensor = CalculateInertiaTensor(rb.mass, collider.shape->getRadius());
        rb.localInverseInertiaTensor = Math::Inverse(rb.localInertiaTensor);
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
        //rb.centreOfMass = pos;

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
        const Math::Vec3 angularAcceleration = rb.localInverseInertiaTensor * rb.torque;

        // Vf = Vi + a(t)
        rb.linearVelocity += linearAcceleration * static_cast<float>(m_timeStep);
        rb.angularVelocity += angularAcceleration * static_cast<float>(m_timeStep);

        pos += rb.linearVelocity * static_cast<float>(m_timeStep);
        rot = Math::Normalized(rot + Math::Quat(0.0f, rb.angularVelocity * static_cast<float>(m_timeStep) * 0.5f) * rot);

        rb.position = pos;

        rb.collisionBody->setTransform(rp3d::Transform(ToRPVec3(rb.position), rp3d::Quaternion(rot.x, rot.y, rot.z, rot.w)));

        rb.rotationMatrix = Math::Mat3Cast(rot);

        // Reset forces and torques
        rb.force = Math::Vec3(0.0f);
        rb.torque = Math::Vec3(0.0f);
    }

    float PhysicsEngine::SolveLambda(const ContactPair& pair)
    {
        auto& contactA = pair.contactA;
        auto& contactB = pair.contactB;

        const Math::Vec3 r1xN = Math::Cross(contactA->relativeLocalContactDistance, pair.contactNormal);
        const Math::Vec3 r2xN = Math::Cross(contactB->relativeLocalContactDistance, pair.contactNormal);

        const float e = (contactA->parentBody->bounciness + contactB->parentBody->bounciness) / 2.0f;
        const float n1 = -(1.0f + e);

        const float n2 = Math::Dot(pair.contactNormal, pair.relativeVelocity);

        const float n3a = Math::Dot(contactA->parentBody->angularVelocity, r1xN);
        const float n3b = Math::Dot(contactB->parentBody->angularVelocity, r2xN);
        const float n3 = n3a - n3b;

        const float numerator = n1 * (n2 + n3);

        const float d1a = contactA->parentBody->mass != 0.0f ? 1.0f / contactA->parentBody->mass : 0.0f;
        const float d1b = contactB->parentBody->mass != 0.0f ? 1.0f / contactB->parentBody->mass : 0.0f;

        const float d1 = d1a + d1b;

        const float d2a = Math::Dot(r1xN, contactA->parentBody->localInverseInertiaTensor * r1xN);
        const float d2b = Math::Dot(r2xN, contactB->parentBody->localInverseInertiaTensor * r2xN);
        const float d2 = d2a + d2b;

        const float denominator = d1 + d2;

        const float lambda = numerator / denominator;
        
        //VK_TRACE("r1: {0} {1} {2}", contactA->relativeLocalContactDistance.x, contactA->relativeLocalContactDistance.y, contactA->relativeLocalContactDistance.z);
        //VK_TRACE("r2: {0} {1} {2}", contactB->relativeLocalContactDistance.x, contactB->relativeLocalContactDistance.y, contactB->relativeLocalContactDistance.z);
        //VK_TRACE("r1crossN: {0} {1} {2}", r1xN.x, r1xN.y, r1xN.z);
        //VK_TRACE("r2crossN: {0} {1} {2}", r2xN.x, r2xN.y, r2xN.z);
        //VK_TRACE("lambda: {0}", lambda);

        return lambda;
    }

    void PhysicsEngine::DetectCollisions(PhysicsScene& scene)
    {
        // Let reactphysics3d handle the collision detection
        scene.Update(m_timeStep);
    }

    void PhysicsEngine::ResolveCollisions(const ContactPair& pair)
    {
        if (!pair.contactA || !pair.contactB)
            return;

        const auto& bodyA = pair.contactA->parentBody;
        const auto& bodyB = pair.contactB->parentBody;

        const Math::Vec3 r1xN = Math::Cross(pair.contactA->relativeLocalContactDistance, pair.contactNormal);
        const Math::Vec3 r2xN = Math::Cross(pair.contactB->relativeLocalContactDistance, pair.contactNormal);

        const Math::Vec3 depenetration = -pair.contactNormal * pair.penetrationDepth;

        VK_TRACE("lambda: {0}", pair.lambda);
        VK_TRACE("impulse: {0} {1} {2}", pair.impulse.x, pair.impulse.y, pair.impulse.z);

        //VK_TRACE("(Body 1): Linear Velocity Before: {0} {1} {2}", bodyA->linearVelocity.x, bodyA->linearVelocity.y, bodyA->linearVelocity.z);

        if (bodyA->type != BodyType::Static)
        {
            bodyA->linearVelocity += pair.impulse / bodyA->mass;
            //bodyA->angularVelocity += pair.lambda * bodyA->localInverseInertiaTensor * r1xN;
        }

        //VK_TRACE("(Body 1): Linear Velocity After: {0} {1} {2}", bodyA->linearVelocity.x, bodyA->linearVelocity.y, bodyA->linearVelocity.z);

        VK_TRACE("(Body 2): Linear Velocity Before: {0} {1} {2}", bodyB->linearVelocity.x, bodyB->linearVelocity.y, bodyB->linearVelocity.z);

        if (bodyB->type != BodyType::Static)
        {
            bodyB->linearVelocity -= pair.impulse / bodyB->mass;
            //bodyB->angularVelocity -= pair.lambda * bodyB->localInverseInertiaTensor * r2xN;
        }

        VK_TRACE("(Body 2): Linear Velocity After: {0} {1} {2}", bodyB->linearVelocity.x, bodyB->linearVelocity.y, bodyB->linearVelocity.z);

        //bodyA->position += depenetration;
        //bodyB->position += depenetration;

        //bodyA->collisionBody->setTransform(rp3d::Transform(ToRPVec3(bodyA->position), bodyA->collisionBody->getTransform().getOrientation()));
        //bodyB->collisionBody->setTransform(rp3d::Transform(ToRPVec3(bodyB->position), bodyB->collisionBody->getTransform().getOrientation()));
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
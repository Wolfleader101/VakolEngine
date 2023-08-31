#include "Physics/PhysicsEngine.hpp"
#include "Physics/PhysicsScene.hpp"

#include "Logger/Logger.hpp"
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
        Math::Vec3 h = FromRPVec3(collider.shape->getHalfExtents());

        rb.inertiaTensor[0][0] = (1.0f / 12.0f) * rb.mass * ((h.y * h.y) + (h.z * h.z));
        rb.inertiaTensor[1][1] = (1.0f / 12.0f) * rb.mass * ((h.x * h.x) + (h.z * h.z));
        rb.inertiaTensor[2][2] = (1.0f / 12.0f) * rb.mass * ((h.x * h.x) + (h.y * h.y));
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

    void PhysicsEngine::ApplyForces(Math::Vec3& pos, Math::Vec3& rot, RigidBody& rb)
    {
        Math::Quat quatRot(Math::DegToRad(rot));

        rb.collisionData->parentBody = &rb;
        // VK_TRACE("RB Address: {0}", fmt::ptr(&ent.GetComponent<RigidBody>()));
        // VK_TRACE("RB Parent Address: {0}", fmt::ptr(rb.collisionData->parentBody));

        if (rb.type == BodyType::Static)
        {
            // Now rotate correctly to world space
            rb.worldInertiaTensor =
                Math::Mat3Cast(quatRot) * rb.inertiaTensor * Math::Transpose(Math::Mat3Cast(quatRot));

            //! THIS IS A HACK TO MAKE SURE THAT THE COLLIDER FOLLOWS THE TRANSFORM
            rb.collisionBody->setTransform(rp3d::Transform(
                rp3d::Vector3(static_cast<double>(pos.x), static_cast<double>(pos.y), static_cast<double>(pos.z)),
                rp3d::Quaternion(static_cast<double>(quatRot.x), static_cast<double>(quatRot.y),
                                 static_cast<double>(quatRot.z), static_cast<double>(quatRot.w))));
            return;
        }

        // can be assumed as static, can be moved later
        static Math::Vec3 gravity(0.0f, -9.8f, 0.0f);

        rb.force += static_cast<float>(rb.mass) * gravity;

        rb.linearVelocity += rb.force * static_cast<float>(m_timeStep);

        pos += rb.linearVelocity * static_cast<float>(m_timeStep);

        // Update angular velocity
        rb.angularVelocity += rb.torque * static_cast<float>(m_timeStep);

        // Convert angular velocity to degrees per time step
        Math::Vec3 angularChange = rb.angularVelocity * static_cast<float>(m_timeStep);

        // Update Euler angles
        rot += angularChange;
        quatRot = Math::Quat(Math::DegToRad(rot));

        rb.worldInertiaTensor = Math::Mat3Cast(quatRot) * rb.inertiaTensor * Math::Transpose(Math::Mat3Cast(quatRot));

        // Update transform with new position
        rb.collisionBody->setTransform(rp3d::Transform(
            rp3d::Vector3(static_cast<double>(pos.x), static_cast<double>(pos.y), static_cast<double>(pos.z)),
            rp3d::Quaternion(static_cast<double>(quatRot.x), static_cast<double>(quatRot.y),
                             static_cast<double>(quatRot.z), static_cast<double>(quatRot.w))));

        // reset the force
        rb.force = Math::Vec3(0.0f, 0.0f, 0.0f);
        rb.torque = Math::Vec3(0.0f, 0.0f, 0.0f);
    }

    void PhysicsEngine::DetectCollisions(PhysicsScene& scene)
    {
        scene.Update(m_timeStep);
    }

    double PhysicsEngine::SolveLambda(RigidBody& rb1, RigidBody& rb2)
    {
        // lambda calcuations
        // w = angular velocity
        // v = linear velocity
        // e = restitution coefficient
        // n = normal
        // r = distance from center of mass to collision point
        // m = mass
        // T = transpose
        // J = intertia tensor, 3x3 matrix

        // v1 is body1 velocity
        // v2 is body 2 velocity

        // lamba = -(1 + e) * (n . (v1 - v2) + w1 . (r1 x n) - w2 . (r2 x n)) / 1/m1 + 1/m2 + ((r1 x n)^T . J1^-1 * (r1
        // x n) + (r2 x n)^T . J2^-1 * (r2 x n))

        // normal
        Math::Vec3 n = rb1.collisionData->worldNormal;

        // distance from collision point to center of mass
        Math::Vec3 r1 = rb1.centerOfMass - rb1.collisionData->localPoint;
        Math::Vec3 r2 = rb2.centerOfMass - rb2.collisionData->localPoint;

        // (r1 x n)
        Math::Vec3 r1CrossN = Math::Cross(r1, n);

        // (r2 x n)
        Math::Vec3 r2CrossN = Math::Cross(r2, n);

        // n . (v1 - v2)
        double nv = Math::Dot(n, Math::Vec3(rb1.linearVelocity - rb2.linearVelocity));

        // w1 . (r1 x n)
        double wr1 = Math::Dot(rb1.angularVelocity, r1CrossN);

        // w2 . (r2 x n)
        double wr2 = Math::Dot(rb2.angularVelocity, r2CrossN);

        double e = (rb1.bounciness + rb2.bounciness) / 2.0; // average

        // top =  -(1 + e) * (n . (v1 - v2) + w1 . (r1 x n) - w2 . (r2 x n))
        double top = -(1.0f + e) * (nv + wr1 - wr2);

        // 1/m1 + 1/m2
        double rb2MassInv = rb2.type == BodyType::Static ? 0.0 : 1.0 / rb2.mass;
        double masses = (1 / rb1.mass) + rb2MassInv;

        // J1^-1
        Math::Mat3 j1Inverse = Math::Inverse(rb1.worldInertiaTensor);

        // j2^-1
        Math::Mat3 j2Inverse = Math::Inverse(rb2.worldInertiaTensor);

        // (r1 x n)^T . J1^-1 * (r1 x n)
        double r1j = Math::Dot(r1CrossN, j1Inverse * r1CrossN);

        // (r2 x n)^T . J2^-1 * (r2 x n)
        double r2j = Math::Dot(r2CrossN, j2Inverse * r2CrossN);

        // bottom = 1/m1 + 1/m2 + ((r1 x n)^T J1^-1 . (r1 x n) + (r2 x n)^T . J2^-1 . (r2 x n))
        double bottom = masses + (r1j + r2j);

        // lambda = top/bottom
        double Lambda = top / bottom;

        return Lambda;
    }

    void PhysicsEngine::ResolveCollisions(Math::Vec3& pos, Math::Vec3& rot, RigidBody& rb)
    {
        // Exit if there's no collision data or if the body is static
        if (!rb.collisionData || rb.type == BodyType::Static)
        {
            return;
        }

        if (!rb.collisionData->isColliding)
            return;

        // distance from collision point to center of mass
        Math::Vec3 r1 = rb.centerOfMass - rb.collisionData->localPoint;

        double Lambda = rb.collisionData->lambda;
        VK_INFO("Lambda: {0}", Lambda);

        Math::Vec3 impulse = rb.accumulatedImpulse;
        VK_INFO("Impulse: {0} {1} {2}", impulse.x, impulse.y, impulse.z);

        // Update linear velocities
        rb.linearVelocity += impulse / static_cast<float>(rb.mass);

        // Update angular velocities
        Math::Vec3 newW = Math::Inverse(rb.inertiaTensor) * Math::Cross(r1, impulse);

        rb.angularVelocity += newW;

        Depenetration(pos, rb);

        // reset the collision data
        rb.collisionData->penetrationDepth = 0.0;
        rb.collisionData->worldNormal = Math::Vec3(0.0f, 0.0f, 0.0f);
        rb.collisionData->worldPoint = Math::Vec3(0.0f, 0.0f, 0.0f);
        rb.collisionData->isColliding = false;
    }

    void PhysicsEngine::Depenetration(Math::Vec3& pos, RigidBody& rb)
    {
        if (!rb.collisionData || rb.type == BodyType::Static)
        {
            return;
        }

        if (!rb.collisionData->isColliding)
        {
            return;
        }

        // Calculate the depenetration vector
        Math::Vec3 depenetration =
            -rb.collisionData->worldNormal * static_cast<float>(rb.collisionData->penetrationDepth);

        pos += depenetration;

        // Update transform with new position
        rb.collisionBody->setTransform(rp3d::Transform(
            rp3d::Vector3(static_cast<double>(pos.x), static_cast<double>(pos.y), static_cast<double>(pos.z)),
            rb.collisionBody->getTransform().getOrientation()));
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
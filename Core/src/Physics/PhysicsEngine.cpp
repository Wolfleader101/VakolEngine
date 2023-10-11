#include "Physics/PhysicsEngine.hpp"

#include "Logger/Logger.hpp"
#include "Physics/PhysicsScene.hpp"

#include "Logger/Logger.hpp"
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

        rp3d::TriangleVertexArray* triangleArray = new rp3d::TriangleVertexArray(
            vertices.size(),          // number of vertices
            vertices.data(),          // start of vertex data
            sizeof(Math::Point),      // stride of vertex data (3 floats per vertex)
            indices.size() / 3,       // number of triangles (assuming 3 indices per triangle)
            indices.data(),           // start of index data
            sizeof(unsigned int) * 3, // stride of index data (3 indices per triangle)
            rp3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE, // vertex data type
            rp3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE  // index data type
        );

        rp3d::TriangleMesh* triangleMesh = m_rpCommon.createTriangleMesh();

        triangleMesh->addSubpart(triangleArray);

        collider.shape = m_rpCommon.createConcaveMeshShape(triangleMesh);
        collider.collider = nullptr;

        VK_WARN("Mesh Collider not supported!");

        return collider;
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, BoxCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());

        Math::Vec3 rpJ = FromRPVec3(collider.shape->getLocalInertiaTensor(rb.mass));

        rb.inertiaTensor[0][0] = rpJ.x;
        rb.inertiaTensor[1][1] = rpJ.y;
        rb.inertiaTensor[2][2] = rpJ.z;
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, SphereCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());

        Math::Vec3 rpJ = FromRPVec3(collider.shape->getLocalInertiaTensor(rb.mass));

        rb.inertiaTensor[0][0] = rpJ.x;
        rb.inertiaTensor[1][1] = rpJ.y;
        rb.inertiaTensor[2][2] = rpJ.z;
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, CapsuleCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());

        Math::Vec3 rpJ = FromRPVec3(collider.shape->getLocalInertiaTensor(rb.mass));

        rb.inertiaTensor[0][0] = rpJ.x;
        rb.inertiaTensor[1][1] = rpJ.y;
        rb.inertiaTensor[2][2] = rpJ.z;
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, MeshCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());

        Math::Vec3 rpJ = FromRPVec3(collider.shape->getLocalInertiaTensor(rb.mass));

        rb.inertiaTensor[0][0] = rpJ.x;
        rb.inertiaTensor[1][1] = rpJ.y;
        rb.inertiaTensor[2][2] = rpJ.z;
    }

    void PhysicsEngine::ApplyForces(Math::Vec3& pos, Math::Quat& quatRot, RigidBody& rb)
    {
        rb.collisionData->parentBody = &rb;

        if (rb.type == BodyType::Static)
        {
            //! THIS IS A HACK TO MAKE SURE THAT THE COLLIDER FOLLOWS THE TRANSFORM
            rb.collisionBody->setTransform(rp3d::Transform(
                rp3d::Vector3(pos.x, pos.y, pos.z), rp3d::Quaternion(quatRot.x, quatRot.y, quatRot.z, quatRot.w)));
            return;
        }

        rb.rotationMatrix = Math::Mat3Cast(quatRot); // Convert the quaternion to a 3x3 rotation matrix
        rb.worldInertiaTensor = rb.rotationMatrix * rb.inertiaTensor * Math::Transpose(rb.rotationMatrix);

        // can be assumed as static, can be moved later
        static Math::Vec3 gravity(0.0f, -9.8f, 0.0f);

        Math::Vec3 weight = rb.mass * gravity;
        rb.force += weight;

        Math::Vec3 linearAcceleration = rb.force / rb.mass;

        rb.linearVelocity += linearAcceleration * m_timeStep;
        pos += rb.linearVelocity * m_timeStep;

        // Update angular velocity
        Math::Vec3 angularAcceleration = Math::Inverse(rb.worldInertiaTensor) * rb.torque;
        rb.angularVelocity += angularAcceleration * m_timeStep;

        quatRot = quatRot + (Math::Quat(0.0f, rb.angularVelocity * m_timeStep * 0.5f) * quatRot);

        quatRot = Math::Normalized(quatRot);

        // Update transform with new position
        rb.collisionBody->setTransform(rp3d::Transform(rp3d::Vector3(pos.x, pos.y, pos.z),
                                                       rp3d::Quaternion(quatRot.x, quatRot.y, quatRot.z, quatRot.w)));

        // reset the force
        rb.force = Math::Vec3(0.0f, 0.0f, 0.0f);
        rb.torque = Math::Vec3(0.0f, 0.0f, 0.0f);
    }

    void PhysicsEngine::DetectCollisions(PhysicsScene& scene)
    {
        scene.Update(static_cast<double>(m_timeStep));
    }

    float PhysicsEngine::SolveLambda(RigidBody& rb1, RigidBody& rb2)
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
        Math::Vec3 n = Math::Normalized(rb1.collisionData->worldNormal);

        // convert normal to object-local space using the transpose of the rotation matrix
        // n = rb1.rotationMatrix * n;

        // distance from collision point to center of mass in local space
        Math::Vec3 r1 = rb1.collisionData->localPoint - rb1.centerOfMass;
        Math::Vec3 r2 = rb2.collisionData->localPoint - rb2.centerOfMass;

        // r1 = rb1.rotationMatrix * r1;
        // r2 = rb2.rotationMatrix * r2;
        VK_ERROR("r1: {0} {1} {2}", r1.x, r1.y, r1.z);
        VK_ERROR("r2: {0} {1} {2}", r2.x, r2.y, r2.z);

        Math::Vec3 v1 = rb1.linearVelocity;
        Math::Vec3 v2 = rb2.linearVelocity;
        Math::Vec3 w1 = rb1.angularVelocity;
        Math::Vec3 w2 = rb2.angularVelocity;

        // (r1 x n)
        Math::Vec3 r1CrossN = Math::Cross(r1, n);

        // (r2 x n)
        Math::Vec3 r2CrossN = Math::Cross(r2, n);

        VK_INFO("r1crossN: {0} {1} {2}", r1CrossN.x, r1CrossN.y, r1CrossN.z);
        VK_INFO("r2crossN: {0} {1} {2}", r2CrossN.x, r2CrossN.y, r2CrossN.z);

        // n . (v1 - v2)
        float nv = Math::Dot(n, Math::Vec3(v1 - v2));

        // w1 . (r1 x n)
        float wr1 = Math::Dot(w1, r1CrossN);

        // w2 . (r2 x n)
        float wr2 = Math::Dot(w2, r2CrossN);

        float e = (rb1.bounciness + rb2.bounciness) / 2.0f; // average
        VK_ERROR("Bounciness: {0}", e);

        // top =  -(1 + e) * (n . (v1 - v2) + w1 . (r1 x n) - w2 . (r2 x n))
        float top = -(1.0f + e) * (nv + wr1 - wr2);

        // 1/m1 + 1/m2
        float rb2MassInv = rb2.type == BodyType::Static ? 0.0f : 1.0f / rb2.mass;
        float masses = (1.0f / rb1.mass) + rb2MassInv;

        // J1^-1
        //! THESE CAN BE STORED AS CONSTANTS
        Math::Mat3 j1Inverse = rb1.type == BodyType::Static ? Math::Mat3(0.0f) : Math::Inverse(rb1.inertiaTensor);
        VK_CRITICAL("J1 (inertia tensor): {0} {1} {2}", rb1.inertiaTensor[0][0], rb1.inertiaTensor[1][1],
                    rb1.inertiaTensor[2][2]);
        VK_CRITICAL("J1^-1: {0} {1} {2}", j1Inverse[0][0], j1Inverse[1][1], j1Inverse[2][2]);

        // j2^-1
        Math::Mat3 j2Inverse = rb2.type == BodyType::Static ? Math::Mat3(0.0f) : Math::Inverse(rb2.inertiaTensor);

        VK_CRITICAL("J2^-1: {0} {1} {2}", j2Inverse[0][0], j2Inverse[1][1], j2Inverse[2][2]);

        // (r1 x n)^T * J1^-1 * (r1 x n)
        float r1j = Math::Dot(r1CrossN, j1Inverse * r1CrossN);

        // (r2 x n)^T . J2^-1 * (r2 x n)
        float r2j = Math::Dot(r2CrossN, j2Inverse * r2CrossN);

        // bottom = 1/m1 + 1/m2 + ((r1 x n)^T J1^-1 . (r1 x n) + (r2 x n)^T . J2^-1 . (r2 x n))
        float bottom = masses + (r1j + r2j);

        // lambda = top/bottom
        float Lambda = top / bottom;

        return Lambda;
    }

    void PhysicsEngine::ResolveCollisions(Math::Vec3& pos, RigidBody& rb)
    {
        // Exit if there's no collision data or if the body is static
        if (!rb.collisionData || rb.type == BodyType::Static)
        {
            return;
        }

        if (!rb.collisionData->isColliding)
            return;

        Math::Vec3& impulse = rb.impulse;
        VK_CRITICAL("Impulse: {0} {1} {2}", impulse.x, impulse.y, impulse.z);

        VK_ERROR("Linear Velocity Before: {0} {1} {2}", rb.linearVelocity.x, rb.linearVelocity.y, rb.linearVelocity.z);

        // Update linear velocities
        rb.linearVelocity += impulse / rb.mass;

        VK_ERROR("Linear Velocity After: {0} {1} {2}", rb.linearVelocity.x, rb.linearVelocity.y, rb.linearVelocity.z);

        VK_WARN("Angular Velocity Before: {0} {1} {2}", rb.angularVelocity.x, rb.angularVelocity.y,
                rb.angularVelocity.z);

        VK_CRITICAL("Torque: {0} {1} {2}", rb.torque.x, rb.torque.y, rb.torque.z);

        rb.angularVelocity += rb.collisionData->lambda * Math::Inverse(rb.inertiaTensor) * rb.torque;

        VK_WARN("Angular Velocity After: {0} {1} {2}", rb.angularVelocity.x, rb.angularVelocity.y,
                rb.angularVelocity.z);

        // reset the collision data
        rb.collisionData->penetrationDepth = 0.0;
        rb.collisionData->worldNormal = Math::Vec3(0.0f, 0.0f, 0.0f);
        rb.collisionData->isColliding = false;
        rb.impulse = Math::Vec3(0.0f, 0.0f, 0.0f);
        rb.collisionData->lambda = 0.0f;
        // rb.torque = Math::Vec3(0.0f, 0.0f, 0.0f);
    }

    void PhysicsEngine::SetTimeStep(float step)
    {
        m_timeStep = step;
    }

    float PhysicsEngine::GetTimeStep() const
    {
        return m_timeStep;
    }

} // namespace Vakol
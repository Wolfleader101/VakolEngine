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
        Math::Mat3 inertiaTensor = Math::Mat3(0.0f);
        inertiaTensor[0][0] = rpJ.x;
        inertiaTensor[1][1] = rpJ.y;
        inertiaTensor[2][2] = rpJ.z;

        if (rb.type == BodyType::Static)
        {
            rb.invInertiaTensor = Math::Vec3(0.0f);
        }
        else
        {
            Math::Mat3 invMat = Math::Inverse(inertiaTensor);
            rb.invInertiaTensor = Math::Vec3(invMat[0][0], invMat[1][1], invMat[2][2]);
        }

        rb.invMass = rb.type == BodyType::Static ? 0.0f : 1.0f / rb.mass;
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, SphereCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());

        Math::Vec3 rpJ = FromRPVec3(collider.shape->getLocalInertiaTensor(rb.mass));
        Math::Mat3 inertiaTensor = Math::Mat3(0.0f);
        inertiaTensor[0][0] = rpJ.x;
        inertiaTensor[1][1] = rpJ.y;
        inertiaTensor[2][2] = rpJ.z;

        if (rb.type == BodyType::Static)
        {
            rb.invInertiaTensor = Math::Vec3(0.0f);
        }
        else
        {
            Math::Mat3 invMat = Math::Inverse(inertiaTensor);
            rb.invInertiaTensor = Math::Vec3(invMat[0][0], invMat[1][1], invMat[2][2]);
        }

        rb.invMass = rb.type == BodyType::Static ? 0.0f : 1.0f / rb.mass;
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, CapsuleCollider& collider)
    {
        const float height = collider.shape->getHeight();

        rp3d::Transform trans =
            rp3d::Transform(rp3d::Vector3(0.0f, height - 0.125f, 0.0f), rp3d::Quaternion::identity());

        collider.collider = rb.collisionBody->addCollider(collider.shape, trans);

        Math::Vec3 rpJ = FromRPVec3(collider.shape->getLocalInertiaTensor(rb.mass));
        Math::Mat3 inertiaTensor = Math::Mat3(0.0f);
        inertiaTensor[0][0] = rpJ.x;
        inertiaTensor[1][1] = rpJ.y;
        inertiaTensor[2][2] = rpJ.z;

        if (rb.type == BodyType::Static)
        {
            rb.invInertiaTensor = Math::Vec3(0.0f);
        }
        else
        {
            Math::Mat3 invMat = Math::Inverse(inertiaTensor);
            rb.invInertiaTensor = Math::Vec3(invMat[0][0], invMat[1][1], invMat[2][2]);
        }

        rb.invMass = rb.type == BodyType::Static ? 0.0f : 1.0f / rb.mass;
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, MeshCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());

        Math::Vec3 rpJ = FromRPVec3(collider.shape->getLocalInertiaTensor(rb.mass));
        Math::Mat3 inertiaTensor = Math::Mat3(0.0f);
        inertiaTensor[0][0] = rpJ.x;
        inertiaTensor[1][1] = rpJ.y;
        inertiaTensor[2][2] = rpJ.z;

        if (rb.type == BodyType::Static)
        {
            rb.invInertiaTensor = Math::Vec3(0.0f);
        }
        else
        {
            Math::Mat3 invMat = Math::Inverse(inertiaTensor);
            rb.invInertiaTensor = Math::Vec3(invMat[0][0], invMat[1][1], invMat[2][2]);
        }

        rb.invMass = rb.type == BodyType::Static ? 0.0f : 1.0f / rb.mass;
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

        if (rb.hasGravity)
        {
            Math::Vec3 weight = rb.mass * gravity;
            rb.force += weight;
        }

        if (rb.isSleeping)
        {
            if (Math::MagnitudeSq(Math::RoundToZero(rb.force)) < 0.01f &&
                Math::MagnitudeSq(Math::RoundToZero(rb.torque)) < 0.01f)
                return;

            rb.isSleeping = false;
            rb.sleepCounter = 0;
        }

        Math::Vec3 linearAcceleration = rb.force * rb.invMass;

        rb.linearVelocity += linearAcceleration * m_timeStep;
        rb.linearVelocity = rb.linearVelocity * velocityDamping;

        rb.linearVelocity.x = rb.lockPosition.x ? 0.0f : rb.linearVelocity.x;
        rb.linearVelocity.y = rb.lockPosition.y ? 0.0f : rb.linearVelocity.y;
        rb.linearVelocity.z = rb.lockPosition.z ? 0.0f : rb.linearVelocity.z;

        Math::Vec3 angularAcceleration = rb.invInertiaTensor * rb.torque;
        rb.angularVelocity += angularAcceleration * m_timeStep;
        rb.angularVelocity = rb.angularVelocity * velocityDamping;

        rb.angularVelocity.x = rb.lockRotation.x ? 0.0f : rb.angularVelocity.x;
        rb.angularVelocity.y = rb.lockRotation.y ? 0.0f : rb.angularVelocity.y;
        rb.angularVelocity.z = rb.lockRotation.z ? 0.0f : rb.angularVelocity.z;

        pos += rb.linearVelocity * m_timeStep;
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

    void PhysicsEngine::SetTimeStep(float step)
    {
        m_timeStep = step;
    }

    float PhysicsEngine::GetTimeStep() const
    {
        return m_timeStep;
    }

} // namespace Vakol
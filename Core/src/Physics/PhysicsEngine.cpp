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

        rb.inverseInertiaTensor = rb.type == BodyType::Static ? Math::Mat3(0.0f) : Math::Inverse(rb.inertiaTensor);
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, SphereCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());

        Math::Vec3 rpJ = FromRPVec3(collider.shape->getLocalInertiaTensor(rb.mass));

        rb.inertiaTensor[0][0] = rpJ.x;
        rb.inertiaTensor[1][1] = rpJ.y;
        rb.inertiaTensor[2][2] = rpJ.z;

        rb.inverseInertiaTensor = rb.type == BodyType::Static ? Math::Mat3(0.0f) : Math::Inverse(rb.inertiaTensor);
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, CapsuleCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());

        Math::Vec3 rpJ = FromRPVec3(collider.shape->getLocalInertiaTensor(rb.mass));

        rb.inertiaTensor[0][0] = rpJ.x;
        rb.inertiaTensor[1][1] = rpJ.y;
        rb.inertiaTensor[2][2] = rpJ.z;

        rb.inverseInertiaTensor = rb.type == BodyType::Static ? Math::Mat3(0.0f) : Math::Inverse(rb.inertiaTensor);
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, MeshCollider& collider)
    {
        collider.collider = rb.collisionBody->addCollider(collider.shape, rp3d::Transform::identity());

        Math::Vec3 rpJ = FromRPVec3(collider.shape->getLocalInertiaTensor(rb.mass));

        rb.inertiaTensor[0][0] = rpJ.x;
        rb.inertiaTensor[1][1] = rpJ.y;
        rb.inertiaTensor[2][2] = rpJ.z;

        rb.inverseInertiaTensor = rb.type == BodyType::Static ? Math::Mat3(0.0f) : Math::Inverse(rb.inertiaTensor);
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
        // Math::Mat3 worldInertiaTensor = rb.rotationMatrix * rb.inertiaTensor * Math::Transpose(rb.rotationMatrix);

        // can be assumed as static, can be moved later
        static Math::Vec3 gravity(0.0f, -9.8f, 0.0f);

        Math::Vec3 weight = rb.mass * gravity;
        rb.force += weight;

        Math::Vec3 linearAcceleration = rb.force / rb.mass;

        rb.linearVelocity += linearAcceleration * m_timeStep;
        pos += rb.linearVelocity * m_timeStep;

        // Update angular velocity
        Math::Vec3 angularAcceleration = rb.inverseInertiaTensor * rb.torque;
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

    void PhysicsEngine::SetTimeStep(float step)
    {
        m_timeStep = step;
    }

    float PhysicsEngine::GetTimeStep() const
    {
        return m_timeStep;
    }

} // namespace Vakol
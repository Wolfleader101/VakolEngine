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

    CollisionShape PhysicsEngine::CreateBoxShape(Math::Vec3& halfExtents)
    {
        CollisionShape box;
        box.shape = m_rpCommon.createBoxShape(rp3d::Vector3(halfExtents.x, halfExtents.y, halfExtents.z));
        box.type = ShapeType::Box;

        return box;
    }

    CollisionShape PhysicsEngine::CreateSphereShape(float radius)
    {
        CollisionShape sphere;
        sphere.shape = m_rpCommon.createSphereShape(radius);
        sphere.type = ShapeType::Sphere;

        return sphere;
    }

    CollisionShape PhysicsEngine::CreateCapsuleShape(float radius, float height)
    {
        CollisionShape capsule;
        capsule.shape = m_rpCommon.createCapsuleShape(radius, height);
        capsule.type = ShapeType::Capsule;

        return capsule;
    }

    CollisionShape PhysicsEngine::CreateMeshShape(std::vector<Math::Point>& vertices,
                                                  std::vector<unsigned int>& indices)
    {
        CollisionShape mesh;
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

        mesh.shape = m_rpCommon.createConcaveMeshShape(triangleMesh);
        mesh.type = ShapeType::TriangleMesh;

        VK_WARN("Mesh Shape not supported!");

        return mesh;
    }

    void PhysicsEngine::CalculateCentreOfMass(RigidBody& rb, CompoundCollider& compoundCollider)
    {
        // calculate the centre of mass with new collider
        Math::Vec3 combinedCM = Math::Vec3(0.0f);
        float combinedMass = 0.0f;

        for (auto& c : compoundCollider.colliders)
        {
            // you need to get the relative position of the collider to the body
            combinedCM += FromRPVec3(c.rpCollider->getLocalToBodyTransform().getPosition()) * c.mass;
            combinedMass += c.mass;
        }

        rb.centerOfMass = combinedCM / combinedMass;

        VK_ERROR("Final COM: {0} {1} {2}", rb.centerOfMass.x, rb.centerOfMass.y, rb.centerOfMass.z);
        rb.mass = combinedMass;
        rb.invMass = 1.0f / rb.mass;
    }

    void PhysicsEngine::CalculateCombinedIntertia(RigidBody& rb, CompoundCollider& compoundCollider)
    {
        // calculalte parrel axis theorem and second moments of inertia coming up
        // I = I + m * d^2

        // calculate inertia tensor for each collider and sum them up
        Math::Vec3 combinedInertiaTensor = Math::Vec3(0.0f);

        // you must sum up the inertia tensors of all the colliders
        for (auto& c : compoundCollider.colliders)
        {
            if (c.shape.type == ShapeType::Box)
            {
                rp3d::BoxShape* box = static_cast<rp3d::BoxShape*>(c.shape.shape);

                float w = box->getHalfExtents().x;
                float h = box->getHalfExtents().y;
                float d = box->getHalfExtents().z;
                float mass = c.mass;

                // inertia tensor about the center of mass of the box
                Math::Vec3 localInertia(1.0f / 12.0f * mass * (h * h + d * d), 1.0f / 12.0f * mass * (w * w + d * d),
                                        1.0f / 12.0f * mass * (w * w + h * h));

                // pos relative to the combined center of mass
                Math::Vec3 relativePos =
                    FromRPVec3(c.rpCollider->getLocalToBodyTransform().getPosition()) - rb.centerOfMass;

                // parallel axis theorem
                localInertia.x += mass * (relativePos.y * relativePos.y + relativePos.z * relativePos.z);
                localInertia.y += mass * (relativePos.x * relativePos.x + relativePos.z * relativePos.z);
                localInertia.z += mass * (relativePos.x * relativePos.x + relativePos.y * relativePos.y);

                // add combined inertia tensor
                combinedInertiaTensor.x += localInertia.x;
                combinedInertiaTensor.y += localInertia.y;
                combinedInertiaTensor.z += localInertia.z;
            }
            else
            {
                // calculate the distance from the colliders center of mass to the new common center of mass
                Math::Vec3 distance =
                    rb.centerOfMass - FromRPVec3(c.rpCollider->getLocalToBodyTransform().getPosition());

                // the interia tensor for the collider
                Math::Vec3 rpJ = FromRPVec3(c.rpCollider->getCollisionShape()->getLocalInertiaTensor(c.mass));

                // calculate the inertia tensor of the collider for the new common center of mass
                // I = I + m * d^2 (for each axis)
                Math::Vec3 partInertiaTensor = rpJ;
                partInertiaTensor.x += c.mass * (distance.y * distance.y + distance.z * distance.z);
                partInertiaTensor.y += c.mass * (distance.x * distance.x + distance.z * distance.z);
                partInertiaTensor.z += c.mass * (distance.x * distance.x + distance.y * distance.y);

                // add to combined inertia tensor
                combinedInertiaTensor += partInertiaTensor;
            }
        }

        Math::Mat3 inertiaTensor = Math::Mat3(0.0f);
        inertiaTensor[0][0] = combinedInertiaTensor.x;
        inertiaTensor[1][1] = combinedInertiaTensor.y;
        inertiaTensor[2][2] = combinedInertiaTensor.z;

        Math::Mat3 invMat = Math::Inverse(inertiaTensor);
        rb.invInertiaTensor = Math::Vec3(invMat[0][0], invMat[1][1], invMat[2][2]);
    }

    void PhysicsEngine::AttachCollider(RigidBody& rb, CompoundCollider& compoundCollider, Math::Vec3& relativePos)
    {
        // get the latest collider
        Vakol::ColliderData& collider = compoundCollider.colliders.back();

        // add the collider to the rigidbody
        collider.rpCollider = rb.collisionBody->addCollider(
            collider.shape.shape, rp3d::Transform(ToRPVec3(relativePos), rp3d::Quaternion::identity()));

        if (rb.type == BodyType::Static)
        {
            rb.invMass = 0.0f;
            rb.centerOfMass = Math::Vec3(0.0f);
            rb.invInertiaTensor = Math::Vec3(0.0f);
            return;
        }

        // calculate the centre of mass with new collider
        CalculateCentreOfMass(rb, compoundCollider);

        // calculate the inertia tensor
        CalculateCombinedIntertia(rb, compoundCollider);
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

        // if (rb.isSleeping && rb.hasGravity)
        // {
        //     rb.isSleeping = false;
        //     rb.sleepCounter = 0;
        // }

        if (rb.hasGravity)
        {
            Math::Vec3 weight = rb.mass * gravity;
            rb.force += weight;
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
#include "Physics/CollisionListener.hpp"
#include "Physics/PhysicsEngine.hpp"

#include "Logger/Logger.hpp"
namespace Vakol
{
    void Depenetration(RigidBody& rb1, RigidBody& rb2, Math::Vec3& normal, float penetrationDepth);
    void Resolution(RigidBody& rb1, RigidBody& rb2, Math::Vec3& normal, Math::Vec3& localPoint1,
                    Math::Vec3& localPoint2);

    void CollisionListener::onContact(const rp3d::CollisionCallback::CallbackData& data)
    {
        for (unsigned int p = 0; p < data.getNbContactPairs(); p++)
        {
            const ContactPair& contactPair = data.getContactPair(p);

            rp3d::CollisionBody* body1 = contactPair.getBody1();
            rp3d::CollisionBody* body2 = contactPair.getBody2();

            if (contactPair.getNbContactPoints() == 0u)
            {
                continue;
            }

            CollisionData* body1Data = static_cast<CollisionData*>(body1->getUserData());
            CollisionData* body2Data = static_cast<CollisionData*>(body2->getUserData());

            if (!body1Data || !body2Data)
            {
                continue;
            }

            RigidBody& rb1 = *body1Data->parentBody;
            RigidBody& rb2 = *body2Data->parentBody;

            rb1.collisionData->isColliding = true;
            rb2.collisionData->isColliding = true;

            // For each contact point of the contact pair
            for (unsigned int c = 0; c < contactPair.getNbContactPoints(); c++)
            {
                // Get the contact point
                CollisionCallback::ContactPoint contactPoint = contactPair.getContactPoint(c);

                Math::Vec3 localPoint1 =
                    Math::Vec3(contactPoint.getLocalPointOnCollider1().x, contactPoint.getLocalPointOnCollider1().y,
                               contactPoint.getLocalPointOnCollider1().z);

                Math::Vec3 localPoint2 =
                    Math::Vec3(contactPoint.getLocalPointOnCollider2().x, contactPoint.getLocalPointOnCollider2().y,
                               contactPoint.getLocalPointOnCollider2().z);

                Math::Vec3 normal = Math::Normalized(FromRPVec3(contactPoint.getWorldNormal()));

                float penetrationDepth = contactPoint.getPenetrationDepth();

                // VK_CRITICAL("N {0}, {1}, {2}", normal.x, normal.y, normal.z);

                // VK_CRITICAL("V1 {0}, {1}, {2}", body1Data->parentBody->linearVelocity.x,
                //             body1Data->parentBody->linearVelocity.y, body1Data->parentBody->linearVelocity.z);

                // VK_CRITICAL("V2 {0}, {1}, {2}", body2Data->parentBody->linearVelocity.x,
                //             body2Data->parentBody->linearVelocity.y, body2Data->parentBody->linearVelocity.z);

                // VK_CRITICAL("M1 {0}", body1Data->parentBody->mass);
                // VK_CRITICAL("M2 {0}", body2Data->parentBody->mass);

                // VK_CRITICAL("W1 {0}, {1}, {2}", body1Data->parentBody->angularVelocity.x,
                //             body1Data->parentBody->angularVelocity.y, body1Data->parentBody->angularVelocity.z);

                // VK_CRITICAL("W2 {0}, {1}, {2}", body2Data->parentBody->angularVelocity.x,
                //             body2Data->parentBody->angularVelocity.y, body2Data->parentBody->angularVelocity.z);

                Math::Vec3 r1 = localPoint1 - body1Data->parentBody->centerOfMass;
                Math::Vec3 r2 = localPoint2 - body2Data->parentBody->centerOfMass;

                Math::Vec3 relativeVel =
                    (body2Data->parentBody->linearVelocity + Math::Cross(body2Data->parentBody->angularVelocity, r2)) -
                    (body1Data->parentBody->linearVelocity + Math::Cross(body1Data->parentBody->angularVelocity, r1));

                Depenetration(rb1, rb2, normal, penetrationDepth);

                // if moving away from each other dont apply impulse
                if (Math::Dot(relativeVel, normal) > 0.0f)
                {
                    return;
                }

                Resolution(rb1, rb2, normal, localPoint1, localPoint2);
            }

            VK_TRACE("Collision");
        }
    }

    void Depenetration(RigidBody& rb1, RigidBody& rb2, Math::Vec3& normal, float penetrationDepth)
    {
        if (rb1.type != BodyType::Static)
        {

            rb1.position += ((-(penetrationDepth / 2)) * normal);

            rb1.collisionBody->setTransform(
                rp3d::Transform(rp3d::Vector3(rb1.position.x, rb1.position.y, rb1.position.z),
                                rb1.collisionBody->getTransform().getOrientation()));
        }

        if (rb2.type != BodyType::Static)
        {
            rb2.position -= ((penetrationDepth / 2) * normal);

            rb2.collisionBody->setTransform(
                rp3d::Transform(rp3d::Vector3(rb2.position.x, rb2.position.y, rb2.position.z),
                                rb2.collisionBody->getTransform().getOrientation()));
        }

        if (rb1.type == BodyType::Static && rb2.type == BodyType::Static)
        {
            return;
        }

        // float rb1MassInv = rb1.type == BodyType::Static ? 0.0f : 1.0f / rb1.mass;
        // float rb2MassInv = rb2.type == BodyType::Static ? 0.0f : 1.0f / rb2.mass;

        // float masses = rb1MassInv + rb2MassInv;

        // if (masses == 0.0f)
        // {
        //     return;
        // }

        // float depth = fmaxf(penetrationDepth - 0.01f, 0.0f);
        // float scalar = depth / masses;
        // Math::Vec3 correction = scalar * normal * 0.45f;

        // rb1.position -= correction * rb1MassInv;
        // rb2.position += correction * rb2MassInv;

        // rb1.collisionBody->setTransform(rp3d::Transform(rp3d::Vector3(rb1.position.x, rb1.position.y,
        // rb1.position.z),
        //                                                 rb1.collisionBody->getTransform().getOrientation()));

        // rb2.collisionBody->setTransform(rp3d::Transform(rp3d::Vector3(rb2.position.x, rb2.position.y,
        // rb2.position.z),
        //                                                 rb2.collisionBody->getTransform().getOrientation()));
    }

    void Resolution(RigidBody& rb1, RigidBody& rb2, Math::Vec3& normal, Math::Vec3& localPoint1,
                    Math::Vec3& localPoint2)
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
        Math::Vec3 n = Math::Normalized(normal);

        // convert normal to object-local space using the transpose of the rotation matrix
        // n = rb1.rotationMatrix * n;

        // distance from collision point to center of mass in local space
        Math::Vec3 r1 = localPoint1 - rb1.centerOfMass;
        Math::Vec3 r2 = localPoint2 - rb2.centerOfMass;

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

        // (r1 x n)^T * J1^-1 * (r1 x n)
        float r1j = Math::Dot(r1CrossN, rb1.inverseInertiaTensor * r1CrossN);

        // (r2 x n)^T . J2^-1 * (r2 x n)
        float r2j = Math::Dot(r2CrossN, rb2.inverseInertiaTensor * r2CrossN);

        // bottom = 1/m1 + 1/m2 + ((r1 x n)^T J1^-1 . (r1 x n) + (r2 x n)^T . J2^-1 . (r2 x n))
        float bottom = masses + (r1j + r2j);

        // lambda = top/bottom
        float lambda = top / bottom;

        if (lambda > 0.0f)
            return;

        Math::Vec3 impulse = lambda * n;
        VK_CRITICAL("Impulse: {0} {1} {2}", impulse.x, impulse.y, impulse.z);

        // VK_ERROR("Rb1 Linear Velocity Before: {0} {1} {2}", rb1.linearVelocity.x, rb1.linearVelocity.y,
        //          rb1.linearVelocity.z);

        // Update linear velocities

        if (rb1.type != BodyType::Static)
        {
            rb1.linearVelocity += impulse / rb1.mass;
            rb1.angularVelocity += lambda * rb1.inverseInertiaTensor * r1CrossN;
        }

        if (rb2.type != BodyType::Static)
        {
            rb2.linearVelocity -= impulse / rb2.mass;
            rb2.angularVelocity -= lambda * rb2.inverseInertiaTensor * r2CrossN;
        }

        // VK_ERROR("Linear Velocity After: {0} {1} {2}", rb.linearVelocity.x, rb.linearVelocity.y,
        // rb.linearVelocity.z);

        // VK_WARN("Angular Velocity Before: {0} {1} {2}", rb.angularVelocity.x, rb.angularVelocity.y,
        //         rb.angularVelocity.z);

        // VK_CRITICAL("R Cross N: {0} {1} {2}", rb.collisionData->rCrossN.x, rb.collisionData->rCrossN.y,
        //             rb.collisionData->rCrossN.z);

        // VK_WARN("Angular Velocity After: {0} {1} {2}", rb.angularVelocity.x, rb.angularVelocity.y,
        //         rb.angularVelocity.z);
    }
} // namespace Vakol
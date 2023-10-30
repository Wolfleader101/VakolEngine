#include "Physics/CollisionListener.hpp"
#include "Physics/PhysicsEngine.hpp"

#include "ECS/Components.hpp" //! EWWW TODO REMOVE THIS
#include "Logger/Logger.hpp"

namespace Vakol
{
    constexpr float SLEEP_LINEAR_THRESHOLD = 0.5f;
    constexpr float SLEEP_ANGULAR_THRESHOLD = 0.5f;
    constexpr int SLEEP_COUNTER_THRESHOLD = 45;

    void Depenetration(RigidBody& rb1, RigidBody& rb2, Math::Vec3& normal, float penetrationDepth);
    void Resolution(RigidBody& rb1, RigidBody& rb2, Math::Vec3& normal, Math::Vec3& localPoint1,
                    Math::Vec3& localPoint2);

    void CollisionListener::setOnCollisionCallback(std::function<void(RigidBody&, RigidBody&)> callback)
    {
        m_onCollisionCallback = callback;
    }

    void CollisionListener::onContact(const rp3d::CollisionCallback::CallbackData& data)
    {
        for (unsigned int p = 0; p < data.getNbContactPairs(); p++)
        {

            const ContactPair& contactPair = data.getContactPair(p);

            if (contactPair.getNbContactPoints() == 0u)
            {
                continue;
            }

            // rp3d::CollisionBody* body1 =
            // contactPair.mWorld.getCollisionBody(contactPair.mContactPair.body1Entity.id); rp3d::CollisionBody* body2
            // = contactPair.mWorld.getCollisionBody(contactPair.mContactPair.body2Entity.id);

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

            if (rb1.collisionData)
            {
                rb1.collisionData->isColliding = true;
                rb1.collisionData->otherBody = &rb2;
            }

            if (rb2.collisionData)
            {
                rb2.collisionData->isColliding = true;
                rb2.collisionData->otherBody = &rb1;
            }

            rb1.isSleeping = false;
            rb2.isSleeping = false;

            m_onCollisionCallback(rb1, rb2);

            Math::Vec3 averageLocalPoint1(0.0f, 0.0f, 0.0f);
            Math::Vec3 averageLocalPoint2(0.0f, 0.0f, 0.0f);
            Math::Vec3 averageNormal(0.0f, 0.0f, 0.0f);
            float averagePenetrationDepth = 0.0f;

            unsigned int numContactPoints = contactPair.getNbContactPoints();

            // For each contact point of the contact pair
            for (unsigned int c = 0; c < contactPair.getNbContactPoints(); c++)
            {
                // Get the contact point
                CollisionCallback::ContactPoint contactPoint = contactPair.getContactPoint(c);

                Math::Vec3 localPoint1 = FromRPVec3(contactPoint.getLocalPointOnCollider1());
                averageLocalPoint1 += localPoint1 * rb1.transform->rot; // take rotation into account

                Math::Vec3 localPoint2 = FromRPVec3(contactPoint.getLocalPointOnCollider2());
                averageLocalPoint2 += localPoint2 * rb2.transform->rot;

                averageNormal += Math::Normalized(FromRPVec3(contactPoint.getWorldNormal()));

                averagePenetrationDepth += contactPoint.getPenetrationDepth();
            }

            averageLocalPoint1 /= static_cast<float>(numContactPoints);
            averageLocalPoint2 /= static_cast<float>(numContactPoints);
            averageNormal = Math::Normalized(averageNormal);
            averagePenetrationDepth /= static_cast<float>(numContactPoints);

            Math::Vec3 r1 = averageLocalPoint1 - rb1.centerOfMass;
            Math::Vec3 r2 = averageLocalPoint2 - rb2.centerOfMass;

            Math::Vec3 relativeVel = (rb2.linearVelocity + Math::Cross(rb2.angularVelocity, r2)) -
                                     (rb1.linearVelocity + Math::Cross(rb1.angularVelocity, r1));

            Depenetration(rb1, rb2, averageNormal, averagePenetrationDepth);

            // if moving away from each other dont apply impulse
            if (Math::Dot(relativeVel, averageNormal) > 0.0f)
            {
                continue;
            }

            Resolution(rb1, rb2, averageNormal, averageLocalPoint1, averageLocalPoint2);

            // VK_CRITICAL("Linear Vel Magnitude Sq: {}", Math::MagnitudeSq(rb2.linearVelocity));

            // VK_WARN("Angular Vel Magnitude Sq: {}", Math::MagnitudeSq(rb2.angularVelocity));

            // Check for rb1
            if (Math::MagnitudeSq(Math::RoundToZero(rb1.linearVelocity)) < SLEEP_LINEAR_THRESHOLD &&
                Math::MagnitudeSq(Math::RoundToZero(rb1.angularVelocity)) < SLEEP_ANGULAR_THRESHOLD)
            {
                rb1.sleepCounter++;
                if (rb1.sleepCounter >= SLEEP_COUNTER_THRESHOLD)
                {
                    rb1.isSleeping = true;
                    rb1.linearVelocity = Math::Vec3(0.0f, 0.0f, 0.0f);
                    rb1.angularVelocity = Math::Vec3(0.0f, 0.0f, 0.0f);
                }
            }
            else
            {
                rb1.sleepCounter = 0;
            }

            // Check for rb2
            if (Math::MagnitudeSq(Math::RoundToZero(rb2.linearVelocity)) < SLEEP_LINEAR_THRESHOLD &&
                Math::MagnitudeSq(Math::RoundToZero(rb2.angularVelocity)) < SLEEP_ANGULAR_THRESHOLD)
            {
                rb2.sleepCounter++;
                if (rb2.sleepCounter >= SLEEP_COUNTER_THRESHOLD)
                {
                    rb2.isSleeping = true;
                    rb2.linearVelocity = Math::Vec3(0.0f, 0.0f, 0.0f);
                    rb2.angularVelocity = Math::Vec3(0.0f, 0.0f, 0.0f);
                }
            }
            else
            {
                rb2.sleepCounter = 0;
            }

            ///m_onCollisionCallback(rb1, rb2);
        }
    }

    void Depenetration(RigidBody& rb1, RigidBody& rb2, Math::Vec3& normal, float penetrationDepth)
    {
        if (rb1.type == BodyType::Static && rb2.type == BodyType::Static)
        {
            return;
        }

        float masses = rb1.invMass + rb2.invMass;

        if (masses == 0.0f)
        {
            return;
        }

        float depth = fmaxf(penetrationDepth - 0.01f, 0.0f);
        float scalar = depth / masses;
        Math::Vec3 correction = scalar * normal * 0.45f;

        rb1.transform->pos -= correction * rb1.invMass;
        rb2.transform->pos += correction * rb2.invMass;

        rb1.collisionBody->setTransform(
            rp3d::Transform(rp3d::Vector3(rb1.transform->pos.x, rb1.transform->pos.y, rb1.transform->pos.z),
                            rb1.collisionBody->getTransform().getOrientation()));

        rb2.collisionBody->setTransform(
            rp3d::Transform(rp3d::Vector3(rb2.transform->pos.x, rb2.transform->pos.y, rb2.transform->pos.z),
                            rb2.collisionBody->getTransform().getOrientation()));
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
        // J = inertia tensor, 3x3 matrix

        // v1 is body 1 velocity
        // v2 is body 2 velocity

        // lamba = -(1 + e) * (n . (v1 - v2) + w1 . (r1 x n) - w2 . (r2 x n)) / 1/m1 + 1/m2 + ((r1 x n)^T . J1^-1 * (r1
        // x n) + (r2 x n)^T . J2^-1 * (r2 x n))

        // normal
        Math::Vec3 n = Math::Normalized(normal);

        // distance from collision point to center of mass in local space
        Math::Vec3 r1 = Math::RoundToZero(localPoint1 - rb1.centerOfMass);
        Math::Vec3 r2 = Math::RoundToZero(localPoint2 - rb2.centerOfMass);

        // VK_ERROR("r1: {0} {1} {2}", r1.x, r1.y, r1.z);
        // VK_ERROR("r2: {0} {1} {2}", r2.x, r2.y, r2.z);

        Math::Vec3 v1 = Math::RoundToZero(rb1.linearVelocity);
        Math::Vec3 v2 = Math::RoundToZero(rb2.linearVelocity);
        Math::Vec3 w1 = Math::RoundToZero(rb1.angularVelocity);
        Math::Vec3 w2 = Math::RoundToZero(rb2.angularVelocity);

        // (r1 x n)
        Math::Vec3 r1CrossN = Math::Cross(r1, n);

        // (r2 x n)
        Math::Vec3 r2CrossN = Math::Cross(r2, n);

        // VK_INFO("r1crossN: {0} {1} {2}", r1CrossN.x, r1CrossN.y, r1CrossN.z);
        // VK_INFO("r2crossN: {0} {1} {2}", r2CrossN.x, r2CrossN.y, r2CrossN.z);

        // n . (v1 - v2)
        float nv = Math::Dot(n, Math::Vec3(v1 - v2));

        // w1 . (r1 x n)
        float wr1 = Math::Dot(w1, r1CrossN);

        // w2 . (r2 x n)
        float wr2 = Math::Dot(w2, r2CrossN);

        float e = (rb1.bounciness + rb2.bounciness) / 2.0f; // average
        // VK_ERROR("Bounciness: {0}", e);

        // top =  -(1 + e) * (n . (v1 - v2) + w1 . (r1 x n) - w2 . (r2 x n))
        float top = -(1.0f + e) * (nv + wr1 - wr2);

        // 1/m1 + 1/m2
        float masses = rb1.invMass + rb2.invMass;

        // (r1 x n)^T * J1^-1 * (r1 x n)
        float r1j = Math::Dot(r1CrossN, rb1.invInertiaTensor * r1CrossN);

        // (r2 x n)^T . J2^-1 * (r2 x n)
        float r2j = Math::Dot(r2CrossN, rb2.invInertiaTensor * r2CrossN);

        // bottom = 1/m1 + 1/m2 + ((r1 x n)^T J1^-1 . (r1 x n) + (r2 x n)^T . J2^-1 . (r2 x n))
        float bottom = masses + (r1j + r2j);

        // lambda = top/bottom
        float lambda = top / bottom;

        if (lambda > 0.0f)
            return;

        Math::Vec3 impulse = lambda * n;
        // VK_CRITICAL("Impulse: {0} {1} {2}", impulse.x, impulse.y, impulse.z);

        if (rb1.type != BodyType::Static)
        {
            rb1.linearVelocity += impulse * rb1.invMass;
            rb1.angularVelocity += lambda * rb1.invInertiaTensor * r1CrossN;
            // VK_CRITICAL("Angular Velocity: {0} {1} {2}", rb1.angularVelocity.x, rb1.angularVelocity.y,
            //             rb1.angularVelocity.z);
        }

        if (rb2.type != BodyType::Static)
        {
            rb2.linearVelocity -= impulse * rb2.invMass;
            rb2.angularVelocity -= lambda * rb2.invInertiaTensor * r2CrossN;
        }
    }
} // namespace Vakol
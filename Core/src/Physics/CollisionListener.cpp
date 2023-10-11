#include "Physics/CollisionListener.hpp"
#include "Physics/PhysicsEngine.hpp"

#include "Logger/Logger.hpp"
namespace Vakol
{

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

            RigidBody& rb1 = *body1Data->parentBody;
            RigidBody& rb2 = *body2Data->parentBody;

            // For each contact point of the contact pair
            for (unsigned int c = 0; c < contactPair.getNbContactPoints(); c++)
            {
                // Get the contact point
                CollisionCallback::ContactPoint contactPoint = contactPair.getContactPoint(c);

                Math::Vec3 localPoint1 =
                    Math::Vec3(contactPoint.getLocalPointOnCollider1().x, contactPoint.getLocalPointOnCollider1().y,
                               contactPoint.getLocalPointOnCollider1().z);

                Math::Vec3 worldPoint1 = FromRPVec3(contactPair.getCollider1()->getLocalToWorldTransform() *
                                                    contactPoint.getLocalPointOnCollider1());

                Math::Vec3 localPoint2 =
                    Math::Vec3(contactPoint.getLocalPointOnCollider2().x, contactPoint.getLocalPointOnCollider2().y,
                               contactPoint.getLocalPointOnCollider2().z);

                Math::Vec3 worldPoint2 = FromRPVec3(contactPair.getCollider2()->getLocalToWorldTransform() *
                                                    contactPoint.getLocalPointOnCollider2());

                Math::Vec3 normal = Math::Normalized(FromRPVec3(contactPoint.getWorldNormal()));

                float penetrationDepth = contactPoint.getPenetrationDepth();

                if (body1Data)
                {
                    body1Data->worldNormal = normal;
                    body1Data->localPoint = localPoint1;
                    body1Data->worldPoint = worldPoint1;
                    body1Data->isColliding = true;
                }

                if (body2Data)
                {
                    body2Data->worldNormal = normal;
                    body2Data->localPoint = localPoint2;
                    body2Data->worldPoint = worldPoint2;
                    body2Data->isColliding = true;
                }

                if (body1Data && body2Data)
                {
                    body1Data->otherBody = body2Data->parentBody;
                    body2Data->otherBody = body1Data->parentBody;

                    VK_CRITICAL("N {0}, {1}, {2}", normal.x, normal.y, normal.z);

                    VK_CRITICAL("V1 {0}, {1}, {2}", body1Data->parentBody->linearVelocity.x,
                                body1Data->parentBody->linearVelocity.y, body1Data->parentBody->linearVelocity.z);

                    VK_CRITICAL("V2 {0}, {1}, {2}", body2Data->parentBody->linearVelocity.x,
                                body2Data->parentBody->linearVelocity.y, body2Data->parentBody->linearVelocity.z);

                    VK_CRITICAL("M1 {0}", body1Data->parentBody->mass);
                    VK_CRITICAL("M2 {0}", body2Data->parentBody->mass);

                    VK_CRITICAL("W1 {0}, {1}, {2}", body1Data->parentBody->angularVelocity.x,
                                body1Data->parentBody->angularVelocity.y, body1Data->parentBody->angularVelocity.z);

                    VK_CRITICAL("W2 {0}, {1}, {2}", body2Data->parentBody->angularVelocity.x,
                                body2Data->parentBody->angularVelocity.y, body2Data->parentBody->angularVelocity.z);

                    Math::Vec3 r1 = localPoint1 - body1Data->parentBody->centerOfMass;
                    Math::Vec3 r2 = localPoint2 - body2Data->parentBody->centerOfMass;

                    Math::Vec3 relativeVel = (body2Data->parentBody->linearVelocity +
                                              Math::Cross(body2Data->parentBody->angularVelocity, r2)) -
                                             (body1Data->parentBody->linearVelocity +
                                              Math::Cross(body1Data->parentBody->angularVelocity, r1));

                    Depenetration(rb1, rb2, normal, penetrationDepth);

                    // if moving away from each other dont apply impulse
                    if (Math::Dot(relativeVel, normal) > 0.0f)
                    {
                        return;
                    }

                    float lambda = PhysicsEngine::SolveLambda(*body1Data->parentBody, *body2Data->parentBody);

                    body1Data->lambda = lambda;
                    body2Data->lambda = lambda;
                    VK_TRACE("Lambda: {}", lambda);

                    body1Data->parentBody->impulse = lambda * normal;
                    body1Data->parentBody->collisionData->lambda = lambda;
                    body2Data->parentBody->impulse = -lambda * normal;
                    body2Data->parentBody->collisionData->lambda = -lambda;

                    body1Data->r = r1;
                    body1Data->rCrossN = Math::Cross(r1, normal);

                    body2Data->r = r2;
                    body2Data->rCrossN = Math::Cross(r2, normal);

                    if (body1Data && body2Data)
                    {
                        PhysicsEngine::IntegrateImpulse(*body1Data->parentBody);
                        PhysicsEngine::IntegrateImpulse(*body2Data->parentBody);

                        // Depenetration(rb1, rb2, normal, penetrationDepth);
                    }
                }
            }

            VK_TRACE("Collision");
        }
    }
} // namespace Vakol
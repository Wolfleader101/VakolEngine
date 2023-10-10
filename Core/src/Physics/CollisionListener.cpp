#include "Physics/CollisionListener.hpp"
#include "Physics/PhysicsEngine.hpp"

#include "Logger/Logger.hpp"
namespace Vakol
{

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
                    body1Data->penetrationDepth = penetrationDepth;
                    body1Data->isColliding = true;
                }

                if (body2Data)
                {
                    body2Data->worldNormal = normal;
                    body2Data->localPoint = localPoint2;
                    body2Data->worldPoint = worldPoint2;
                    body2Data->penetrationDepth = penetrationDepth;
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

                    float lambda = PhysicsEngine::SolveLambda(*body1Data->parentBody, *body2Data->parentBody);
                    body1Data->lambda = lambda;
                    body2Data->lambda = lambda;
                    VK_TRACE("Lambda: {}", lambda);

                    body1Data->parentBody->impulse += lambda * normal;
                    body1Data->parentBody->collisionData->lambda += lambda;
                    body2Data->parentBody->impulse += -lambda * normal;
                    body2Data->parentBody->collisionData->lambda += -lambda;

                    if (body1Data->parentBody->type != BodyType::Static)
                    {

                        body1Data->parentBody->position =
                            body1Data->parentBody->position + ((-(penetrationDepth / 2)) * normal);

                        Math::Vec3& pos = body1Data->parentBody->position;

                        body1->setTransform(rp3d::Transform(rp3d::Vector3(pos.x, pos.y, pos.z),
                                                            body1->getTransform().getOrientation()));
                    }

                    if (body2Data->parentBody->type != BodyType::Static)
                    {
                        body2Data->parentBody->position =
                            body2Data->parentBody->position - ((penetrationDepth / 2) * normal);

                        Math::Vec3& pos = body2Data->parentBody->position;

                        body2->setTransform(rp3d::Transform(rp3d::Vector3(pos.x, pos.y, pos.z),
                                                            body2->getTransform().getOrientation()));
                    }
                }
            }

            if (body1Data && body2Data)
            {
                PhysicsEngine::ResolveCollisions(body1Data->parentBody->position, *body1Data->parentBody);
                PhysicsEngine::ResolveCollisions(body2Data->parentBody->position, *body2Data->parentBody);
            }
            VK_TRACE("Collision");
        }
    }
} // namespace Vakol
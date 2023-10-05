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

                Math::Vec3 worldPoint2 = FromRPVec3(contactPair.getCollider2()->getLocalToWorldTransform() *
                                                    contactPoint.getLocalPointOnCollider2());

                Math::Vec3 localPoint2 =
                    Math::Vec3(contactPoint.getLocalPointOnCollider2().x, contactPoint.getLocalPointOnCollider2().y,
                               contactPoint.getLocalPointOnCollider2().z);

                Math::Vec3 normal = Math::Normalized(FromRPVec3(contactPoint.getWorldNormal()));

                float penetrationDepth = contactPoint.getPenetrationDepth();

                if (body1Data)
                {
                    body1Data->worldNormal = normal;
                    body1Data->worldPoint = worldPoint1;
                    body1Data->localPoint = localPoint1;
                    body1Data->penetrationDepth = penetrationDepth;
                    body1Data->isColliding = true;
                }

                if (body2Data)
                {
                    body2Data->worldNormal = normal;
                    body2Data->worldPoint = worldPoint2;
                    body2Data->localPoint = localPoint2;
                    body2Data->penetrationDepth = penetrationDepth;
                    body2Data->isColliding = true;
                }

                if (body1Data && body2Data)
                {
                    body1Data->otherBody = body2Data->parentBody;
                    body2Data->otherBody = body1Data->parentBody;

                    float lambda = PhysicsEngine::SolveLambda(*body1Data->parentBody, *body2Data->parentBody);
                    body1Data->lambda = lambda;
                    body2Data->lambda = lambda;
                    VK_TRACE("Lambda: {}", lambda);

                    body1Data->parentBody->impulse += lambda * normal;
                    body2Data->parentBody->impulse += -lambda * normal;
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
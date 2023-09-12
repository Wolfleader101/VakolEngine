#include "Physics/CollisionListener.hpp"
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

            CollisionData* body1Data = static_cast<CollisionData*>(body1->getUserData());
            CollisionData* body2Data = static_cast<CollisionData*>(body2->getUserData());

            Math::Vec3 avgNormal = Math::Vec3(0.0f, 0.0f, 0.0f);

            Math::Vec3 avgWorldPoint1 = Math::Vec3(0.0f, 0.0f, 0.0f);
            Math::Vec3 avgLocalPoint1 = Math::Vec3(0.0f, 0.0f, 0.0f);

            Math::Vec3 avgWorldPoint2 = Math::Vec3(0.0f, 0.0f, 0.0f);
            Math::Vec3 avgLocalPoint2 = Math::Vec3(0.0f, 0.0f, 0.0f);

            float avgPenetrationDepth = 0.0f;

            if (contactPair.getNbContactPoints() == 0u)
            {
                continue;
            }

            // For each contact point of the contact pair
            for (unsigned int c = 0; c < contactPair.getNbContactPoints(); c++)
            {
                // Get the contact point
                ContactPoint contactPoint = contactPair.getContactPoint(c);

                rp3d::Vector3 worldPoint1 =
                    contactPair.getCollider1()->getLocalToWorldTransform() * contactPoint.getLocalPointOnCollider1();

                avgWorldPoint1 += Math::Vec3(worldPoint1.x, worldPoint1.y, worldPoint1.z);
                avgLocalPoint1 +=
                    Math::Vec3(contactPoint.getLocalPointOnCollider1().x, contactPoint.getLocalPointOnCollider1().y,
                               contactPoint.getLocalPointOnCollider1().z);

                rp3d::Vector3 worldPoint2 =
                    contactPair.getCollider2()->getLocalToWorldTransform() * contactPoint.getLocalPointOnCollider2();

                avgWorldPoint2 += Math::Vec3(worldPoint2.x, worldPoint2.y, worldPoint2.z);
                avgLocalPoint2 +=
                    Math::Vec3(contactPoint.getLocalPointOnCollider2().x, contactPoint.getLocalPointOnCollider2().y,
                               contactPoint.getLocalPointOnCollider2().z);

                rp3d::Vector3 rp3dNormal = contactPoint.getWorldNormal();

                Math::Vec3 normal(rp3dNormal.x, rp3dNormal.y, rp3dNormal.z);

                avgNormal += normal;

                avgPenetrationDepth += contactPoint.getPenetrationDepth();
            }

            avgWorldPoint1 /= static_cast<float>(contactPair.getNbContactPoints());
            avgLocalPoint1 /= static_cast<float>(contactPair.getNbContactPoints());
            avgWorldPoint2 /= static_cast<float>(contactPair.getNbContactPoints());
            avgLocalPoint2 /= static_cast<float>(contactPair.getNbContactPoints());
            avgPenetrationDepth /= static_cast<float>(contactPair.getNbContactPoints());

            if (body1Data)
            {
                body1Data->worldNormal = Math::Normalized(avgNormal);
                body1Data->worldPoint = avgWorldPoint1;
                body1Data->localPoint = avgLocalPoint1;
                body1Data->penetrationDepth += avgPenetrationDepth;
                body1Data->isColliding = true;
            }

            if (body2Data)
            {
                body2Data->worldNormal = -Math::Normalized(avgNormal);
                body2Data->worldPoint = avgWorldPoint2;
                body2Data->localPoint = avgLocalPoint2;
                body2Data->penetrationDepth += avgPenetrationDepth;
                body2Data->isColliding = true;
            }

            // if (body1Data && body2Data)
            // {
            //     double lambda = PhysicsEngine::SolveLambda(*body1Data->parentBody, *body2Data->parentBody);
            //     body1Data->lambda += lambda;
            //     body2Data->lambda += lambda;
            // }
        }
    }
} // namespace Vakol
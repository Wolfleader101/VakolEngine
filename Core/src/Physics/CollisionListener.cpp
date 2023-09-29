#include "Physics/CollisionListener.hpp"
#include "Logger/Logger.hpp"

namespace Vakol
{

    void CollisionListener::onContact(const CallbackData& data)
    {
        for (unsigned int p = 0; p < data.getNbContactPairs(); p++)
        {
            const ContactPair& pair = data.getContactPair(p);

            rp3d::CollisionBody* body1 = pair.getBody1();
            rp3d::CollisionBody* body2 = pair.getBody2();

            auto* b1Data = static_cast<CollisionData*>(body1->getUserData());
            auto* b2Data = static_cast<CollisionData*>(body2->getUserData());

            rp3d::Vector3 worldPoint1 = rp3d::Vector3::zero();
            rp3d::Vector3 worldPoint2 = rp3d::Vector3::zero();

            rp3d::Vector3 worldNormal = rp3d::Vector3::zero();

            rp3d::Vector3 localPoint1 = rp3d::Vector3::zero();
            rp3d::Vector3 localPoint2 = rp3d::Vector3::zero();

            rp3d::Vector3 localNormal1 = rp3d::Vector3::zero();
            rp3d::Vector3 localNormal2 = rp3d::Vector3::zero();

            float penetrationDepth = 0.0f;

            for (unsigned int c = 0; c < pair.getNbContactPoints(); ++c)
            {
                const ContactPoint& point = pair.getContactPoint(c);

                worldPoint1 = pair.getCollider1()->getLocalToWorldTransform() * point.getLocalPointOnCollider1();
                worldPoint2 = pair.getCollider2()->getLocalToWorldTransform() * point.getLocalPointOnCollider2();

                localPoint1 = point.getLocalPointOnCollider1();
                localPoint2 = point.getLocalPointOnCollider2();

                worldNormal = point.getWorldNormal();

                localNormal1 = pair.getCollider1()->getLocalToWorldTransform().getInverse() * point.getWorldNormal();
                localNormal2 = pair.getCollider2()->getLocalToWorldTransform().getInverse() * point.getWorldNormal();

                penetrationDepth = point.getPenetrationDepth();
            }

            if (b1Data)
            {
                b1Data->worldPoint = FromRPVec3(worldPoint1);
                b1Data->localPoint = FromRPVec3(localPoint1);

                b1Data->worldNormal = FromRPVec3(worldNormal);
                b1Data->localNormal = FromRPVec3(localNormal1);

                b1Data->penetrationDepth = penetrationDepth;
                b1Data->isColliding = true;
            }

            if (b2Data)
            {
                b2Data->worldPoint = FromRPVec3(worldPoint2);
                b2Data->localPoint = FromRPVec3(localPoint2);

                b2Data->worldNormal = FromRPVec3(worldNormal);
                b2Data->localNormal = FromRPVec3(localNormal2);

                b2Data->penetrationDepth = penetrationDepth;
                b2Data->isColliding = true;
            }
        }
    }
} // namespace Vakol
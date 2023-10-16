#include "Physics/CollisionListener.hpp"
#include "Physics/PhysicsEngine.hpp"

#include "Logger/Logger.hpp"

namespace Vakol
{
    void CollisionListener::onContact(const CallbackData& data)
    {
        for (unsigned int p = 0; p < data.getNbContactPairs(); ++p)
        {
            const ContactPair& pair = data.getContactPair(p);

            if (pair.getNbContactPoints() == 0u)
                continue;

            const auto* contactA = static_cast<ContactData*>(pair.getBody1()->getUserData());
            const auto* contactB = static_cast<ContactData*>(pair.getBody2()->getUserData());

            if (!contactA || !contactB)
                continue;

            RigidBody& bodyA = *contactA->parentBody;
            RigidBody& bodyB = *contactB->parentBody;

            for (unsigned int c = 0; c < pair.getNbContactPoints(); ++c)
            {
                const ContactPoint& point = pair.getContactPoint(c);

                Math::Vec3 localContactPoint1 = FromRPVec3(point.getLocalPointOnCollider1());
                Math::Vec3 localContactPoint2 = FromRPVec3(point.getLocalPointOnCollider2());

                //Math::Vec3 worldContactPoint1 = FromRPVec3(pair.getCollider1()->getLocalToWorldTransform() * point.getLocalPointOnCollider1());
                //Math::Vec3 worldContactPoint2 = FromRPVec3(pair.getCollider2()->getLocalToWorldTransform() * point.getLocalPointOnCollider2());

                Math::Vec3 worldContactNormal = Math::Normalized(FromRPVec3(point.getWorldNormal()));
                const float penetrationDepth = point.getPenetrationDepth();

                PhysicsEngine::Depenetration(bodyA, bodyB, penetrationDepth, worldContactNormal);

                PhysicsEngine::ResolveCollisions(bodyA, bodyB, worldContactNormal, localContactPoint1, localContactPoint2);
            }
        }
    }
} // namespace Vakol
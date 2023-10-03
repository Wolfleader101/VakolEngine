#include "Physics/CollisionListener.hpp"
#include "Physics/PhysicsEngine.hpp"

#include "Logger/Logger.hpp"

namespace Vakol
{
    void Reset(ContactPair& contactPair);
    void Reset(ContactData* contactData);

    void CollisionListener::onContact(const CallbackData& data)
    {
        for (unsigned int p = 0; p < data.getNbContactPairs(); ++p)
        {
            const ContactPair& pair = data.getContactPair(p);
            Vakol::ContactPair contactPair = {};

            auto* contactA = static_cast<ContactData*>(pair.getBody1()->getUserData());
            auto* contactB = static_cast<ContactData*>(pair.getBody2()->getUserData());

            rp3d::Vector3 worldContactPoint1 = rp3d::Vector3::zero();
            rp3d::Vector3 worldContactPoint2 = rp3d::Vector3::zero();

            rp3d::Vector3 localContactPoint1 = rp3d::Vector3::zero();
            rp3d::Vector3 localContactPoint2 = rp3d::Vector3::zero();

            rp3d::Vector3 worldContactNormal = rp3d::Vector3::zero();

            float penetrationDepth = 0.0f;

            for (unsigned int c = 0; c < pair.getNbContactPoints(); ++c)
            {
                const ContactPoint& point = pair.getContactPoint(c);

                localContactPoint1 = point.getLocalPointOnCollider1();
                localContactPoint2 = point.getLocalPointOnCollider2();

                worldContactPoint1 = pair.getCollider1()->getLocalToWorldTransform() * point.getLocalPointOnCollider1();
                worldContactPoint2 = pair.getCollider2()->getLocalToWorldTransform() * point.getLocalPointOnCollider2();

                worldContactNormal = point.getWorldNormal();
                penetrationDepth = point.getPenetrationDepth();

                if (pair.getEventType() == ContactPair::EventType::ContactStart || pair.getEventType() == ContactPair::EventType::ContactStay)
                {
                    if (contactA)
                    {
                        contactA->localContactPoint = FromRPVec3(localContactPoint1);
                        contactA->worldContactPoint = FromRPVec3(worldContactPoint1);

                        contactA->relativeLocalContactPoint = contactA->parentBody->centreOfMass - contactA->localContactPoint;
                        contactA->relativeWorldContactPoint = contactA->parentBody->centreOfMass - contactA->worldContactPoint;

                        contactA->velocity = contactA->parentBody->linearVelocity;
                    }

                    if (contactB)
                    {
                        contactB->localContactPoint = FromRPVec3(localContactPoint2);
                        contactB->worldContactPoint = FromRPVec3(worldContactPoint2);

                        contactB->relativeLocalContactPoint = contactB->parentBody->centreOfMass - contactB->localContactPoint;
                        contactB->relativeWorldContactPoint = contactB->parentBody->centreOfMass - contactB->worldContactPoint;

                        contactB->velocity = contactB->parentBody->linearVelocity;
                    }

                    if (contactA && contactB)
                    {
                        contactPair.contactA = contactA;
                        contactPair.contactB = contactB;

                        contactPair.relativeVelocity = contactA->velocity - contactB->velocity;

                        contactPair.worldContactNormal = FromRPVec3(worldContactNormal);

                        contactPair.penetrationDepth = penetrationDepth;

                        contactPair.impulseSum += PhysicsEngine::SolveImpulse(contactPair);

                        contactPair.contactCount = pair.getNbContactPoints();
                        contactPair.isColliding = true;
                    }
                }
            }

            if (pair.getEventType() == ContactPair::EventType::ContactStart)
            {
                VK_TRACE("ENTER COLLISION");
            }
            else if (pair.getEventType() == ContactPair::EventType::ContactExit)
            {
                VK_TRACE("EXIT COLLISION");

                if (contactA)
                    Reset(contactA);

                if (contactB)
                    Reset(contactB);

                if (contactA && contactB)
                    Reset(contactPair);
            }
        }
    }

    void Reset(ContactPair& contactPair)
    {
        contactPair.contactA = nullptr;
        contactPair.contactB = nullptr;

        contactPair.worldContactNormal = Math::Vec3(0.0f);
        contactPair.relativeVelocity = Math::Vec3(0.0f);
        contactPair.impulseSum = Math::Vec3(0.0f);

        contactPair.penetrationDepth = 0.0f;

        contactPair.contactCount = 0u;
        contactPair.isColliding = false;
    }

    void Reset(ContactData* contactData)
    {
        contactData->contactPair = nullptr;

        contactData->localContactPoint = Math::Vec3(0.0f);
        contactData->worldContactPoint = Math::Vec3(0.0f);

        contactData->relativeLocalContactPoint = Math::Vec3(0.0f);
        contactData->relativeWorldContactPoint = Math::Vec3(0.0f);

        contactData->velocity = Math::Vec3(0.0f);
    }
} // namespace Vakol
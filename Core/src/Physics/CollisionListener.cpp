#include "Physics/CollisionListener.hpp"
#include "Physics/PhysicsEngine.hpp"

#include "Logger/Logger.hpp"

namespace Vakol
{
    void Reset(ContactData* data);

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

                if (pair.getEventType() == ContactPair::EventType::ContactStart)
                {
                    if (contactA)
                    {
                        contactA->velocity = contactA->parentBody->linearVelocity;
                        contactA->position = contactA->parentBody->position;

                        contactA->localContactPoint = FromRPVec3(localContactPoint1);
                        contactA->worldContactPoint = FromRPVec3(worldContactPoint1);

                        contactA->relativeLocalContactDistance = contactA->localContactPoint - contactA->parentBody->centreOfMass;
                        contactA->relativeWorldContactDistance = contactA->worldContactPoint - contactA->parentBody->centreOfMass;
                    }

                    if (contactB)
                    {
                        contactB->velocity = contactB->parentBody->linearVelocity;
                        contactB->position = contactB->parentBody->position;

                        contactB->localContactPoint = FromRPVec3(localContactPoint2);
                        contactB->worldContactPoint = FromRPVec3(worldContactPoint2);

                        contactB->relativeLocalContactDistance = contactB->localContactPoint - contactB->parentBody->centreOfMass;
                        contactB->relativeWorldContactDistance = contactB->worldContactPoint - contactB->parentBody->centreOfMass;
                    }

                    if (contactA && contactB)
                    {
                        contactPair.contactA = contactA;
                        contactPair.contactB = contactB;

                        contactPair.contactNormal = Math::Normalized(FromRPVec3(worldContactNormal));
                        contactPair.relativeVelocity = contactA->velocity - contactB->velocity;

                        contactPair.lambda = PhysicsEngine::SolveLambda(contactPair);
                        contactPair.impulse += contactPair.lambda * contactPair.contactNormal * static_cast<float>(contactPair.contactCount);

                        contactPair.penetrationDepth = penetrationDepth;

                        contactPair.contactCount = pair.getNbContactPoints();
                        contactPair.isColliding = true;
                    }
                }
                else if (pair.getEventType() == ContactPair::EventType::ContactExit)
                {
                    if (contactA)
                        Reset(contactA);
                    if (contactB)
                        Reset(contactB);
                }
            }

            if (pair.getEventType() == ContactPair::EventType::ContactStart)
            {
                VK_TRACE("ENTER CONTACT");
                PhysicsEngine::ResolveCollisions(contactPair);
            }

            if (pair.getEventType() == ContactPair::EventType::ContactExit)
            {
                VK_TRACE("EXIT CONTACT");
            }
        }
    }

    void Reset(ContactData* data)
    {
        data->velocity = Math::Vec3(0.0f);
        data->position = Math::Vec3(0.0f);

        data->localContactPoint = Math::Vec3(0.0f);
        data->worldContactPoint = Math::Vec3(0.0f);

        data->relativeLocalContactDistance = Math::Vec3(0.0f);
        data->relativeWorldContactDistance = Math::Vec3(0.0f);
    }
} // namespace Vakol
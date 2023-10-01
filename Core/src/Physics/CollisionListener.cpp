#include "Physics/CollisionListener.hpp"
#include "Physics/PhysicsEngine.hpp"

#include "Logger/Logger.hpp"

namespace Vakol
{
    void Reset(ContactPair& contactPair);
    void Reset(ContactData* contactData);

    Math::Vec3 impulse = Math::Vec3(0.0f);

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
                    impulse = Math::Vec3(0.0f);
                }
                if (pair.getEventType() == ContactPair::EventType::ContactStart || pair.getEventType() == ContactPair::EventType::ContactStay)
                {
                    if (contactA)
                    {
                        contactA->localContactPoint = FromRPVec3(localContactPoint1);
                        contactA->worldContactPoint = FromRPVec3(worldContactPoint1);

                        contactA->relativeLocalContactPoint = contactA->parentBody->centreOfMass - contactA->localContactPoint;
                        contactA->relativeWorldContactPoint = contactA->parentBody->centreOfMass - contactA->worldContactPoint;

                        contactA->velocity = contactA->parentBody->linearVelocity + Math::Cross(contactA->parentBody->angularVelocity, contactA->localContactPoint);
                    }

                    if (contactB)
                    {
                        contactB->localContactPoint = FromRPVec3(localContactPoint2);
                        contactB->worldContactPoint = FromRPVec3(worldContactPoint2);

                        contactB->relativeLocalContactPoint = contactB->parentBody->centreOfMass - contactB->localContactPoint;
                        contactB->relativeWorldContactPoint = contactB->parentBody->centreOfMass - contactB->worldContactPoint;

                        contactB->velocity = contactB->parentBody->linearVelocity + Math::Cross(contactB->parentBody->angularVelocity, contactB->localContactPoint);
                    }

                    if (contactA && contactB)
                    {
                        contactPair.contactA = contactA;
                        contactPair.contactB = contactB;

                        contactPair.relativeVelocity = contactB->velocity - contactA->velocity;

                        contactPair.worldContactNormal = FromRPVec3(worldContactNormal);

                        contactPair.penetrationDepth = penetrationDepth;

                        contactPair.contactCount = pair.getNbContactPoints();
                        contactPair.isColliding = true;

                        impulse += Vakol::PhysicsEngine::SolveImpulse(contactPair);

                        if (contactA->parentBody->type != BodyType::Static)
                            contactA->parentBody->linearVelocity -= impulse * 1.0f;
                        if (contactB->parentBody->type != BodyType::Static)
                            contactB->parentBody->linearVelocity += impulse * 1.0f;
                    }
                }
                else if (pair.getEventType() == ContactPair::EventType::ContactStay)
                {
                }
                else if (pair.getEventType() == ContactPair::EventType::ContactExit)
                {
                    if (contactA)
                        Reset(contactA);

                    if (contactB)
                        Reset(contactB);

                    if (contactA && contactB)
                        Reset(contactPair);

                    impulse = Math::Vec3(0.0f);
                }
            }
        }
    }

    void Reset(ContactPair& contactPair)
    {
        contactPair.contactA = nullptr;
        contactPair.contactB = nullptr;

        contactPair.worldContactNormal = Math::Vec3(0.0f);

        contactPair.relativeVelocity = Math::Vec3(0.0f);

        contactPair.penetrationDepth = 0.0f;

        contactPair.contactCount = 0u;
        contactPair.isColliding = false;
    }

    void Reset(ContactData* contactData)
    {
        contactData->localContactPoint = Math::Vec3(0.0f);
        contactData->worldContactPoint = Math::Vec3(0.0f);

        contactData->relativeLocalContactPoint = Math::Vec3(0.0f);
        contactData->relativeWorldContactPoint = Math::Vec3(0.0f);

        contactData->velocity = Math::Vec3(0.0f);
    }
} // namespace Vakol
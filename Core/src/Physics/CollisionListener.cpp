#include "Physics/CollisionListener.hpp"
#include "Physics/PhysicsEngine.hpp"

#include "Logger/Logger.hpp"

namespace Vakol
{
    void Reset(ContactData* data);

    void CollisionListener::onContact(const CallbackData& data)
    {
        rp3d::Vector3 worldContactPoint1 = rp3d::Vector3::zero();
        rp3d::Vector3 worldContactPoint2 = rp3d::Vector3::zero();

        rp3d::Vector3 localContactPoint1 = rp3d::Vector3::zero();
        rp3d::Vector3 localContactPoint2 = rp3d::Vector3::zero();

        rp3d::Vector3 worldContactNormal = rp3d::Vector3::zero();

        float penetrationDepth = 0.0f;

        for (unsigned int p = 0; p < data.getNbContactPairs(); ++p)
        {
            const ContactPair& pair = data.getContactPair(p);

            if (pair.getNbContactPoints() == 0u)
                continue;

            auto* contactA = static_cast<ContactData*>(pair.getBody1()->getUserData());
            auto* contactB = static_cast<ContactData*>(pair.getBody2()->getUserData());

            Vakol::ContactPair contactPair = {};

            for (unsigned int c = 0; c < pair.getNbContactPoints(); ++c)
            {
                const ContactPoint& point = pair.getContactPoint(c);

                localContactPoint1 = point.getLocalPointOnCollider1();
                localContactPoint2 = point.getLocalPointOnCollider2();

                worldContactPoint1 = pair.getCollider1()->getLocalToWorldTransform() * point.getLocalPointOnCollider1();
                worldContactPoint2 = pair.getCollider2()->getLocalToWorldTransform() * point.getLocalPointOnCollider2();

                worldContactNormal = point.getWorldNormal();
                penetrationDepth = point.getPenetrationDepth();

                if (contactA)
                {
                    contactA->localContactPoint = FromRPVec3(localContactPoint1);
                    contactA->worldContactPoint = FromRPVec3(worldContactPoint1);

                    contactA->relativeLocalContactDistance = contactA->localContactPoint - contactA->parentBody->centreOfMass;
                    contactA->relativeWorldContactDistance = contactA->worldContactPoint - contactA->parentBody->centreOfMass;

                    contactA->velocity = contactA->parentBody->linearVelocity + Math::Cross(contactA->parentBody->angularVelocity, contactA->relativeLocalContactDistance);

                    contactA->isColliding = true;
                }

                if (contactB)
                {
                    contactB->localContactPoint = FromRPVec3(localContactPoint2);
                    contactB->worldContactPoint = FromRPVec3(worldContactPoint2);

                    contactB->relativeLocalContactDistance = contactB->localContactPoint - contactB->parentBody->centreOfMass;
                    contactB->relativeWorldContactDistance = contactB->worldContactPoint - contactB->parentBody->centreOfMass;

                    contactB->velocity = contactB->parentBody->linearVelocity + Math::Cross(contactB->parentBody->angularVelocity, contactB->relativeLocalContactDistance);

                    contactB->isColliding = true;
                }

                if (contactA && contactB)
                {
                    contactPair.contactA = contactA;
                    contactPair.contactB = contactB;

                    contactPair.contactNormal = Math::Normalized(FromRPVec3(worldContactNormal));
                    contactPair.relativeVelocity = contactA->velocity - contactB->velocity;

                    contactPair.lambda = PhysicsEngine::SolveLambda(contactPair);
                    contactPair.impulse = contactPair.lambda * contactPair.contactNormal;

                    contactPair.penetrationDepth = penetrationDepth;

                    contactPair.contactCount = pair.getNbContactPoints();

                    if (contactA->parentBody->type != BodyType::Static)
                    {
                        contactA->parentBody->linearVelocity += contactPair.impulse * contactA->parentBody->GetInverseMass();
                        contactA->parentBody->angularVelocity += contactPair.lambda * contactA->parentBody->inverseInertiaTensor * Math::Cross(contactA->relativeLocalContactDistance, contactPair.contactNormal);
                    }

                    if (contactB->parentBody->type != BodyType::Static)
                    {
                        contactB->parentBody->linearVelocity -= contactPair.impulse * contactB->parentBody->GetInverseMass();
                        contactB->parentBody->angularVelocity -= contactPair.lambda * contactB->parentBody->inverseInertiaTensor * Math::Cross(contactB->relativeLocalContactDistance, contactPair.contactNormal);
                    }
                }

                if (pair.getEventType() == ContactPair::EventType::ContactExit)
                {
                    if (contactA)
                        Reset(contactA);
                    if (contactB)
                        Reset(contactB);
                }
            }
        }
    }

    void Reset(ContactData* data)
    {
        data->localContactPoint = Math::Vec3(0.0f);
        data->worldContactPoint = Math::Vec3(0.0f);

        data->relativeLocalContactDistance = Math::Vec3(0.0f);
        data->relativeWorldContactDistance = Math::Vec3(0.0f);

        data->velocity = Math::Vec3(0.0f);

        data->isColliding = false;
    }
} // namespace Vakol
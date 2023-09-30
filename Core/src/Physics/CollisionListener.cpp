#include "Physics/CollisionListener.hpp"
#include "Logger/Logger.hpp"

namespace Vakol
{
    void Reset(CollisionData* data);

    void CollisionListener::onContact(const CallbackData& data)
    {
        for (unsigned int p = 0; p < data.getNbContactPairs(); ++p)
        {
            const ContactPair& pair = data.getContactPair(p);

            auto* b1Data = static_cast<CollisionData*>(pair.getBody1()->getUserData());
            auto* b2Data = static_cast<CollisionData*>(pair.getBody2()->getUserData());

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
            }

            if (pair.getEventType() == ContactPair::EventType::ContactStart)
            {
                //VK_TRACE("ENTER");
            }
            else if (pair.getEventType() == ContactPair::EventType::ContactStart || pair.getEventType() == ContactPair::EventType::ContactStay)
            {
                if (b1Data)
                {
                    b1Data->otherBody = b2Data->parentBody;

                    if (b1Data->parentBody->type == BodyType::Dynamic && b1Data->otherBody->type == BodyType::Static)
                    {
                        // Calculate relative velocity with body1 as reference
                        b1Data->relativeVelocity =
                            b1Data->parentBody->linearVelocity - b1Data->otherBody->linearVelocity;
                    }
                    else if (b1Data->parentBody->type == BodyType::Static && b1Data->otherBody->type == BodyType::Dynamic)
                    {
                        // Calculate relative velocity with body2 as reference
                        b1Data->relativeVelocity =
                            b1Data->otherBody->linearVelocity - b1Data->parentBody->linearVelocity;
                    }
                    b1Data->relativeVelocity = b1Data->otherBody->linearVelocity - b1Data->parentBody->linearVelocity;

                    b1Data->localContactPoint = FromRPVec3(localContactPoint1);
                    b1Data->worldContactPoint = FromRPVec3(worldContactPoint1);

                    b1Data->worldContactNormal = FromRPVec3(worldContactNormal);

                    b1Data->penetrationDepth = penetrationDepth;
                    b1Data->contactCount = pair.getNbContactPoints();

                    b1Data->isColliding = true;
                }

                if (b2Data)
                {
                    b2Data->otherBody = b1Data->parentBody;

                    if (b2Data->parentBody->type == BodyType::Dynamic && b2Data->otherBody->type == BodyType::Static)
                    {
                        // Calculate relative velocity with body1 as reference
                        b2Data->relativeVelocity =
                            b2Data->parentBody->linearVelocity - b2Data->otherBody->linearVelocity;
                    }
                    else if (b2Data->parentBody->type == BodyType::Static && b2Data->otherBody->type == BodyType::Dynamic)
                    {
                        // Calculate relative velocity with body2 as reference
                        b2Data->relativeVelocity =
                            b2Data->otherBody->linearVelocity - b2Data->parentBody->linearVelocity;
                    }

                    b2Data->localContactPoint = FromRPVec3(localContactPoint2);
                    b2Data->worldContactPoint = FromRPVec3(worldContactPoint2);

                    b2Data->worldContactNormal = FromRPVec3(worldContactNormal);

                    b2Data->penetrationDepth = penetrationDepth;
                    b2Data->contactCount = pair.getNbContactPoints();

                    b2Data->isColliding = true;
                }

                // VK_TRACE("Entering!");
            }
            else if (pair.getEventType() == ContactPair::EventType::ContactExit)
            {
                if (b1Data)
                {
                    Reset(b1Data);
                }

                if (b2Data)
                {
                    Reset(b2Data);
                }

                //VK_TRACE("EXIT");
            }
        }
    }

    void Reset(CollisionData* data)
    {
        data->otherBody = nullptr;

        data->relativeVelocity = Math::Vec3(0.0f);

        data->localContactPoint = Math::Vec3(0.0f);
        data->worldContactPoint = Math::Vec3(0.0f);

        data->worldContactNormal = Math::Vec3(0.0f);

        data->penetrationDepth = 0.0f;
        data->contactCount = 0u;

        data->isColliding = false;
    }
} // namespace Vakol
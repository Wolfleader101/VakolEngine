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

            // For each contact point of the contact pair
            for (unsigned int c = 0; c < contactPair.getNbContactPoints(); c++)
            {

                // Get the contact point
                CollisionCallback::ContactPoint contactPoint = contactPair.getContactPoint(c);

                rp3d::Vector3 worldPoint1 =
                    contactPair.getCollider1()->getLocalToWorldTransform() * contactPoint.getLocalPointOnCollider1();

                rp3d::Vector3 worldPoint2 =
                    contactPair.getCollider2()->getLocalToWorldTransform() * contactPoint.getLocalPointOnCollider2();

                // Fetch the collision normal from the contact point
                rp3d::Vector3 rp3dNormal = contactPoint.getWorldNormal();

                // Convert rp3d::Vector3 to Math::Vec3
                Math::Vec3 normal((float)rp3dNormal.x, (float)rp3dNormal.y, (float)rp3dNormal.z);

                if (body1Data)
                {
                    body1Data->normal = normal;
                    body1Data->worldPoint =
                        Math::Vec3((float)worldPoint1.x, (float)worldPoint1.y, (float)worldPoint1.z);
                    body1Data->penetrationDepth = contactPoint.getPenetrationDepth();
                }

                if (body2Data)
                {
                    body2Data->normal = normal;
                    body2Data->worldPoint =
                        Math::Vec3((float)worldPoint2.x, (float)worldPoint2.y, (float)worldPoint2.z);
                }
            }
        }
    }
} // namespace Vakol
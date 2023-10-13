#include "Physics/PhysicsMath.hpp"

namespace Vakol::PhysicsMath
{
    Math::Vec3 PhysicsMath::Acceleration(Math::Vec3 previousVelocity, Math::Vec3 currentVelocity, float deltaTime)
    {
        return ((currentVelocity - previousVelocity) / deltaTime);
    }

    Math::Vec3 PhysicsMath::AngularVelocity(Math::Vec3 previousAngles, Math::Vec3 currentAngles, float deltaTime)
    {
        return ((currentAngles - previousAngles) / deltaTime);
    }

    // Technically does the same thing as angular velocity, but I'm creating a separate function for it for readability
    Math::Vec3 PhysicsMath::LinearVelocity(Math::Vec3 previousPosition, Math::Vec3 currentPosition, float deltaTime)
    {
        return ((currentPosition - previousPosition) / deltaTime);
    }
} // namespace Vakol::PhysicsMath

#pragma once

#include <Math/Math.hpp>

namespace Vakol::PhysicsMath
{
    /**
     * @brief Calculates the acceleration of an object
     *
     * @param previousVelocity The velocity of the object at the previous frame
     * @param currentVelocity The velocity of the object at the current frame
     * @param deltaTime The time between the previous frame and the current frame
     */
    Math::Vec3 Acceleration(Math::Vec3 previousVelocity, Math::Vec3 currentVelocity, float deltaTime);

    /**
     * @brief Calculates the angular velocity of an object
     *
     * @param previousAngles The angles of the object at the previous frame
     * @param currentAngles The angles of the object at the current frame
     * @param deltaTime The time between the previous frame and the current frame
     */
    Math::Vec3 AngularVelocity(Math::Vec3 previousAngles, Math::Vec3 currentAngles, float deltaTime);

    /**
     * @brief Calculates the linear velocity of an object
     *
     * @param previousPosition The position of the object at the previous frame
     * @param currentPosition The position of the object at the current frame
     * @param deltaTime The time between the previous frame and the current frame
     */
    Math::Vec3 LinearVelocity(Math::Vec3 previousPosition, Math::Vec3 currentPosition, float deltaTime); 
}
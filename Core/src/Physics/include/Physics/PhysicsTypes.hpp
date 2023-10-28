#pragma once

#include <memory>
#include <reactphysics3d/reactphysics3d.h>

#include "Math/Math.hpp"

namespace Vakol
{
    inline Math::Vec3 FromRPVec3(rp3d::Vector3& vec)
    {
        return Math::Vec3(vec.x, vec.y, vec.z);
    }

    inline Math::Vec3 FromRPVec3(const rp3d::Vector3& vec)
    {
        return Math::Vec3(vec.x, vec.y, vec.z);
    }

    inline rp3d::Vector3 ToRPVec3(Math::Vec3& vec)
    {
        return rp3d::Vector3(vec.x, vec.y, vec.z);
    }

    namespace Components
    {
        struct Transform;
    }

    struct RigidBody;

    struct RayCastHitInfo
    {
        Math::Vec3 point = Math::Vec3(0.0f, 0.0f, 0.0f);
        Math::Vec3 normal = Math::Vec3(0.0f, 0.0f, 0.0f);

        float distance = 0.0f;
        bool isHit = false;

        RigidBody* rigidbody = nullptr;
    };

    /**
     * @brief Rigidbody types
     *
     */
    enum class BodyType
    {
        Static,
        Kinematic,
        Dynamic
    };

    /**
     * @brief Collision data of rigidbody
     *
     */
    struct CollisionData
    {
        bool isColliding = false;

        RigidBody* parentBody = nullptr;
        RigidBody* otherBody = nullptr;
    };

    /**
     * @brief Rigidbody object
     *
     */
    struct RigidBody
    {
        BodyType type = BodyType::Dynamic;
        float mass = 1.0f;
        float invMass = 1.0f;
        float bounciness = 0.3f;

        bool hasGravity = true;
        bool isSleeping = false;
        int sleepCounter = 0;

        Components::Transform* transform = nullptr;

        Math::Vec3 centerOfMass = Math::Vec3(0.0f, 0.0f, 0.0f); // will have to be calculated

        Math::Vec3 force = Math::Vec3(0.0f, 0.0f, 0.0f);
        Math::Vec3 torque = Math::Vec3(0.0f, 0.0f, 0.0f);
        Math::Vec3 linearVelocity = Math::Vec3(0.0f, 0.0f, 0.0f);
        Math::Vec3 angularVelocity = Math::Vec3(0.0f, 0.0f, 0.0f);

        Math::BVec3 lockPosition = Math::BVec3(false, false, false);
        Math::BVec3 lockRotation = Math::BVec3(false, false, false);

        Math::Vec3 invInertiaTensor = Math::Vec3(0.0f);

        rp3d::CollisionBody* collisionBody = nullptr;

        std::shared_ptr<CollisionData> collisionData = nullptr;

        std::string tag;
    };

    struct CollisionShape
    {
        rp3d::CollisionShape* shape = nullptr;
    };

    struct ColliderData
    {
        rp3d::Collider* rpCollider = nullptr;
        CollisionShape shape;
        float mass;
    };

    struct CompoundCollider
    {
        std::vector<ColliderData> colliders;
    };

} // namespace Vakol

//     /**
//  * @brief Struct representing a Collider.
//  */
// struct Collider
// {
//     /**
//      * @brief Enum for the name of the shape of the collider.
//      */
//     enum ShapeName
//     {
//         BOX,
//         SPHERE,
//         CAPSULE,
//         TRIANGLE_MESH
//     };

//     /**
//      * @brief Struct representing the bounds of a collider.
//      */
//     struct Bounds
//     {
//         rp3d::Vector3 min = {0, 0, 0};              /**< minimum vertice*/
//         rp3d::Vector3 max = {1, 1, 1};              /**< Maximum vertice*/
//         rp3d::Vector3 center = {0.5f, 0.5f, 0.5f};  /**< Average of all vertices*/
//         rp3d::Vector3 extents = {0.5f, 0.5f, 0.5f}; /**< Extent of vertices*/
//         rp3d::Vector3 size = {1, 1, 1};             /**< Size of vertices*/

//         double radius = 0.5 * extents.length(); /**< Radius*/

//         /**
//          * @brief Serializes the bounds of the collider.
//          * @param ar Archive to serialize the data to.
//          */
//         template <class Archive>
//         void serialize(Archive& ar)
//         {
//             ar(cereal::make_nvp("min.x", min.x), cereal::make_nvp("min.y", min.y),
//                cereal::make_nvp("min.z", min.z));
//             ar(cereal::make_nvp("max.x", max.x), cereal::make_nvp("max.y", max.y),
//                cereal::make_nvp("max.z", max.z));
//             ar(cereal::make_nvp("centre.x", center.x), cereal::make_nvp("centre.y", center.y),
//                cereal::make_nvp("centre.z", center.z));
//             ar(cereal::make_nvp("extents.x", extents.x), cereal::make_nvp("extents.y", extents.y),
//                cereal::make_nvp("extents.z", extents.z));
//             ar(cereal::make_nvp("size.x", size.x), cereal::make_nvp("size.y", size.y),
//                cereal::make_nvp("size.z", size.z));
//             ar(cereal::make_nvp("radius", radius));
//         }
//     };

//     /**
//      * @brief Default constructor for the Collider struct.
//      */
//     Collider() = default;

//     /**
//      * @brief Overloaded constructor that initializes a Collider with the owner and the bounds.
//      * @param owner The owner of the collider.
//      * @param data The bounds of the collider.
//      */
//     Collider(RigidBody& owner, const std::optional<Bounds>& data);

//     RigidBody* OwningBody = nullptr;       ///< Pointer to the body owning the collider.
//     rp3d::Collider* ColliderPtr = nullptr; ///< Pointer to the collider.
//     rp3d::CollisionShape* Shape = nullptr; ///< Pointer to the shape of the collider.
//     ShapeName ShapeName = BOX;             ///< The name of the shape of the collider. Default is BOX.
//     bool DrawableBounds = false;           ///< Boolean indicating if the bounds are drawable.

//     Bounds bounds; ///< The bounds of the collider.

//     /**
//      * @brief Sets the bounds for the collider.
//      * @param data The bounds to be set.
//      */
//     void SetBounds(const Bounds& data);

//     /**
//      * @brief Serializes the collider.
//      * @param ar Archive to serialize the data to.
//      */
//     template <class Archive>
//     void serialize(Archive& ar)
//     {
//         ar(cereal::make_nvp("CollisionShape", ShapeName));

//         ar(cereal::make_nvp("Bounds", bounds));
//     }
// };

//     /**
//  * @brief Function to get the bounds of a collider.
//  * @param model Reference to the model of the Drawable.
//  * @param transform Reference to the Transform.
//  * @return Bounds of the collider.
//  */
// Collider::Bounds GetBounds(const Drawable& model, const Transform& transform);

//     Collider::Bounds GetBounds(const Drawable& model, const Transform& transform)
// {
//     Collider::Bounds bounds;

//     rp3d::Vector3& max = bounds.max;
//     rp3d::Vector3& min = bounds.min;

//     const Math::Mat4 transformMat = to_rp3d_mat4(transform);

//     auto& vertices = model.model_ptr->meshes().begin()->vertices();

//     if (vertices.size() < 3)
//     {
//         VK_CRITICAL("Collider::Bounds::GetBounds() - Insufficient vertices data");
//         return bounds;
//     }

//     const auto& position = vertices.begin()->position;

//     const rp3d::Vector3 transformedPosition =
//         transformVertex(transformMat, rp3d::Vector3(position.x, position.y, position.z));

//     max = min = transformedPosition;

//     for (const auto& msh : model.model_ptr->c_meshes())
//     {
//         vertices = msh.c_vertices();

//         for (const auto& vertex : vertices)
//         {
//             const auto temp = transformVertex(transformMat, to_rp3d(vertex.position));
//             max = rp3d::Vector3::max(max, temp);
//             min = rp3d::Vector3::min(min, temp);
//         }
//     }

//     bounds.center = (max + min) / 2.0f;
//     bounds.extents = (max - min) / 2.0f;
//     bounds.size = bounds.extents * 2;
//     bounds.radius = bounds.extents.length() / 2.0f;

//     return bounds;
// }
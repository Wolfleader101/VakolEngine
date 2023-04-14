#pragma once

#include <reactphysics3d/reactphysics3d.h>

#include <Controller/LuaState.hpp>
#include <Controller/Physics/ScenePhysics.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>

#include <Model/Assets/Model.hpp>

namespace Vakol::Model::Components {
    /**
     * @struct Transform
     *
     * @brief store the position, rotation, and scale of an entity
     *
     */
    struct Transform {
        /**
         * @brief Construct a new Transform object
         *
         */
        Transform() = default;

        /**
         * @brief Copy constructor
         *
         * @param otherTransform
         */
        Transform(const Transform& otherTransform) = default;

        /**
         * @brief Construct a new Transform object
         *
         * @param pos posiiton
         * @param rot rotation
         * @param sca scale
         */
        Transform(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scale);

        glm::vec3 pos;   /**< XYZ position */
        glm::quat rot;   /**< XYZ rotation */
        glm::vec3 scale = { 1,1,1 }; /**< XYZ scale */

        template <class Archive>
        void serialize(Archive& ar) {
            ar(cereal::make_nvp("pos.x", pos.x), cereal::make_nvp("pos.y", pos.y), cereal::make_nvp("pos.z", pos.z),

               cereal::make_nvp("rot.w", rot.w), cereal::make_nvp("rot.x", rot.x), cereal::make_nvp("rot.y", rot.y), cereal::make_nvp("rot.z", rot.z),

               cereal::make_nvp("scale.x", scale.x), cereal::make_nvp("scale.y", scale.y), cereal::make_nvp("scale.z", scale.z));
        }
    };

    // using ModelType = VakolModel;

    /**
     * @struct Tag
     *
     *
     * @brief Component storing a string describing the entity
     *
     */
    struct Tag {
        /**
         * @brief Returns bool indicating if string is empty
         *
         */
        bool IsEmpty() { return tag.size() == 0; }

        Tag() = default;
        Tag(const std::string&);

        std::string tag; /**< String object containing data*/

        template <class Archive>
        void serialize(Archive& ar) {
            ar(cereal::make_nvp("tag", tag));
        }
    };

    /**
     * @enum EntityType
     *
     * @brief enum value indicating the type of entity it is
     */
    enum EntityType { UNKNOWN, WORLD, PLAYER, ENEMY, FRIEND, COLLECTABLE, EXIT };

    /**
     *
     * @struct TagType
     *
     * @brief struct containing EntityType enum indicating the type of entity.
     *
     */
    struct TagType {
        EntityType type; /**< Type of entity */

        TagType() = default;
        TagType(uint8_t);

        template <class Archive>
        void serialize(Archive& ar) {
            ar(cereal::make_nvp("TagType", type));
        }
    };

    /**
     * @struct Script
     *
     * @brief script component that holds reference to file
     */
    struct Script {
        std::string script_name;

        Script() = default;
        Script(const std::string& name);

        Script(const std::string& script, Controller::LuaState& lua);

        template <class Archive>
        void serialize(Archive& ar) {
            ar(cereal::make_nvp("ScriptName", script_name));
        }
    };

    struct Drawable {

        Drawable() = default;
        Drawable(std::string&& file);
        std::string name; //for serialization
        std::shared_ptr<Vakol::Model::Assets::Model> ModelPtr = nullptr;

        template<class Archive>
        void serialize(Archive& ar)
        {
            ar(cereal::make_nvp("Model", name));
        }
    };

    using namespace Vakol::Controller::Physics;

    struct PhysicsObject {

        PhysicsObject() = default; //Don't use this. Need it for serialization
        PhysicsObject(std::shared_ptr<ScenePhysics> SP, unsigned int BodyType, unsigned int Shape);

        struct RigidData
        {
            double mass = 3;			/**< Mass of object*/
            bool grav = true;				/**< If gravity is enabled on the object*/
            double LDamp = 0;			/**< Linear Dampening*/
            double ADamp = 2;			/**< Angular Dampening*/
            rp3d::Vector3 AngularLock = { 0,1,0 }; /**< Angular lock axis factor */
            rp3d::Vector3 Orientation = { 0,0,0 }; /**< Orientation */

        };

        //rigid body
        std::shared_ptr<ScenePhysics> owningWorld;
        rp3d::RigidBody* RigidBody;
        rp3d::BodyType Type;
        RigidData Data;

        //collider
        rp3d::Collider* CollisionBody;
        rp3d::CollisionShape* Shape;
        rp3d::CollisionShapeName ShapeName;

        rp3d::Vector3 offset = { 0.0f, 0.0f, 0.0f }; /**< Offset given to the collider*/
        

        
        rp3d::Transform prevTransform;

        template <class Archive>
        void serialize(Archive& ar) 
        {
            ar(cereal::make_nvp("CollisionShape", ShapeName));

            ar(cereal::make_nvp("Offset X", offset.x));
            ar(cereal::make_nvp("Offset Y", offset.y));
            ar(cereal::make_nvp("Offset Z", offset.z));

            ar(cereal::make_nvp("Mass", Data.mass));
            ar(cereal::make_nvp("Gravity", Data.grav));
            ar(cereal::make_nvp("Linear Dampening", Data.LDamp));
            ar(cereal::make_nvp("Angular Dampening", Data.ADamp));
            ar(cereal::make_nvp("BodyType", Type));

            ar(cereal::make_nvp("Angular Lock X", Data.AngularLock.x));
            ar(cereal::make_nvp("Angular Lock Y", Data.AngularLock.y));
            ar(cereal::make_nvp("Angular Lock Z", Data.AngularLock.z));

            ar(cereal::make_nvp("Orientation X", Data.Orientation.x));
            ar(cereal::make_nvp("Orientation Y", Data.Orientation.y));
            ar(cereal::make_nvp("Orientation Z", Data.Orientation.z));

        }
    };

}  // namespace Vakol::Model::Components

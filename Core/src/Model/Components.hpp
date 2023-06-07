#pragma once

#include <reactphysics3d/reactphysics3d.h>

#include <Controller/LuaState.hpp>
#include <Controller/Physics/ScenePhysics.hpp>

#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm/glm.hpp>
#pragma warning(pop)

#include <memory>
#include <string>

#include "Entity.hpp"
#include "Model/Assets/Model.hpp"
#include "Controller/Animator.hpp"

namespace Vakol::Model::Components
{
    /**
     * @struct Transform
     *
     * @brief store the position, rotation, and scale of an entity
     *
     */
    struct Transform
	{
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
         * @param scale
         */
        Transform(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scale);

        glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f); /**< XYZ position */

        glm::vec3 eulerAngles = glm::vec3(0.0f, 0.0f, 0.0f);

        glm::quat rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); /**< WXYZ rotation */

        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f); /**< XYZ scale */

        template <class Archive>
        void serialize(Archive& ar) {
            ar(cereal::make_nvp("pos.x", pos.x), cereal::make_nvp("pos.y", pos.y), cereal::make_nvp("pos.z", pos.z),


               cereal::make_nvp("rot.x", eulerAngles.x), cereal::make_nvp("rot.y", eulerAngles.y), cereal::make_nvp("rot.z", eulerAngles.z),

               cereal::make_nvp("scale.x", scale.x), cereal::make_nvp("scale.y", scale.y),
               cereal::make_nvp("scale.z", scale.z));
        }
    };

    struct Animator
    {
        std::string attached_model;

        void Update(const int state, const float delta_time) { animator_ptr->Update(state, delta_time); }
        void Update(const float delta_time) { animator_ptr->Update(delta_time); }

        const Model::Assets::Animation& const animation(const int state) const { return animator_ptr->get(state); } 

        void set(const std::shared_ptr<Controller::Animator>& animator) { animator_ptr = animator; }
        void set(const Controller::Animator& animator) { animator_ptr = std::make_shared<Controller::Animator>(animator); }

        template <class Archive>
        void serialize(Archive& ar) 
        {
            ar(cereal::make_nvp("attached_model", attached_model));
        }
    private:
        std::shared_ptr<Controller::Animator> animator_ptr = nullptr;
    };

    struct Animation
    {
        int state = 0;
        std::string attached_model;

        template <class Archive>
        void serialize(Archive& ar) 
        {
            ar(cereal::make_nvp("state", state));
            ar(cereal::make_nvp("attached_model", attached_model));
        }
    };

    /**
     * @struct Tag
     *
     *
     * @brief Component storing a string describing the entity
     *
     */
    struct Tag
	{
        /**
         * @brief Returns bool indicating if string is empty
         *
         */
        [[nodiscard]] bool IsEmpty() const { return tag.empty(); }

        Tag() = default;
        explicit Tag(std::string&);

        std::string tag; /**< String object containing data*/

        template <class Archive>
        void serialize(Archive& ar) {
            ar(cereal::make_nvp("tag", tag));
        }
    };

    /**
     * @enum ENTITY_TYPE
     *
     * @brief enum value indicating the type of entity it is
     */
    enum ENTITY_TYPE { UNKNOWN, WORLD, PLAYER, ENEMY, FRIEND, COLLECTABLE, EXIT };

    /**
     *
     * @struct TagType
     *
     * @brief struct containing EntityType enum indicating the type of entity.
     *
     */
    struct TagType {
        ENTITY_TYPE type; /**< Type of entity */

        TagType() = default;
        explicit TagType(uint8_t);

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
        sol::table state;

        Script() = default;
        explicit Script(std::string& name);

        Script(const std::string& script, std::shared_ptr<Controller::LuaState> lua, Entity& entity, Controller::Scene& scene);

        template <class Archive>
        void serialize(Archive& ar) {
            ar(cereal::make_nvp("ScriptName", script_name));
            //ar(cereal::make_nvp("State Table",state));
        }
    };

    /**
     * @brief a finite state machine that can be controlled in lua
     *
     */
    struct FSM
	{
        FSM() = default;
		FSM(std::shared_ptr<Controller::LuaState> lua);

        void AddState(const std::string& stateName, const sol::function& callback);

        void ChangeState(const std::string& stateName);

        std::string GetState();

        void Update();

        template <class Archive>
        void serialize(Archive& ar) 
        {
            ar(cereal::make_nvp("Current State", currentState));
        }


    private:
        std::string currentState;
        sol::table states;
        std::shared_ptr<Controller::LuaState>  lua;


    };

    struct Drawable
    {
        Drawable() = default;
        explicit Drawable(std::string&& file);
        Drawable(const std::string& file, float scale, bool animated, bool backfaceCull);

        std::string name;  // for serialization

        float scale = 1.0f;
        bool animated = false;
        bool backfaceCull = true;
        bool instance = false;

        std::shared_ptr<Assets::Model> model_ptr;

        template <class Archive>
        void serialize(Archive& ar) {
            ar(cereal::make_nvp("Model", name));
            ar(cereal::make_nvp("Import Scale", scale));
            ar(cereal::make_nvp("Animated", animated));
            ar(cereal::make_nvp("Back Face Culling", backfaceCull));
        }
    };

    using namespace Vakol::Controller::Physics;

    struct RigidBody
    {
        enum BODY_TYPE
        {
            STATIC = rp3d::BodyType::STATIC,
            KINEMATIC = rp3d::BodyType::KINEMATIC,
            DYNAMIC = rp3d::BodyType::DYNAMIC
        };

        RigidBody() = default;

        bool initialized = false;

        struct RigidData
        {
            float mass = 3;                        /**< Mass of object*/
            bool grav = true;                      /**< If gravity is enabled on the object*/
            float LDamp = 0;                       /**< Linear Dampening*/
            float ADamp = 1;                       /**< Angular Dampening*/
            rp3d::Vector3 AngularLock = {0, 1, 0}; /**< Angular lock axis factor */
            rp3d::Vector3 Orientation = {0, 0, 0}; /**< Orientation */
        };

        void SetRigidData(const RigidData& data);
        void ToggleGravity();
        void SetBodyType(BODY_TYPE t);
        void SetVelocity(const glm::vec3& vel) const;
        void SetAngularVelocity(const glm::vec3& vel) const;
        void SetLinearDamp(float damp) const;
        void SetAngularDamp(float damp) const;

        // rigid body
        std::shared_ptr<ScenePhysics> owningWorld = nullptr;
        rp3d::RigidBody* RigidBodyPtr = nullptr;
        BODY_TYPE Type = DYNAMIC;  // default
        RigidData Data;

        rp3d::Transform prevTransform;

        template <class Archive>
        void serialize(Archive& ar) {
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

    struct Collider
    {
        enum ShapeName { BOX, SPHERE, CAPSULE, TRIANGLE_MESH };

        struct Bounds {
            rp3d::Vector3 min = {0, 0, 0};              /**< minimum vertice*/
            rp3d::Vector3 max = {1, 1, 1};              /**< Maximum vertice*/
            rp3d::Vector3 center = {0.5f, 0.5f, 0.5f};  /**< Average of all vertices*/
            rp3d::Vector3 extents = {0.5f, 0.5f, 0.5f}; /**< Extent of vertices*/
            rp3d::Vector3 size = {1, 1, 1};             /**< Size of vertices*/

            float radius = 0.5f * extents.length(); /**< Radius*/

            template <class Archive>
            void serialize(Archive& ar) {
                ar(cereal::make_nvp("min.x", min.x), cereal::make_nvp("min.y", min.y),
                   cereal::make_nvp("min.z", min.z));
                ar(cereal::make_nvp("max.x", max.x), cereal::make_nvp("max.y", max.y),
                   cereal::make_nvp("max.z", max.z));
                ar(cereal::make_nvp("centre.x", center.x), cereal::make_nvp("centre.y", center.y),
                   cereal::make_nvp("centre.z", center.z));
                ar(cereal::make_nvp("extents.x", extents.x), cereal::make_nvp("extents.y", extents.y),
                   cereal::make_nvp("extents.z", extents.z));
                ar(cereal::make_nvp("size.x", size.x), cereal::make_nvp("size.y", size.y),
                   cereal::make_nvp("size.z", size.z));
                ar(cereal::make_nvp("radius", radius));
            }
        };

        Collider() = default;
        Collider(RigidBody& owner, const std::optional<Bounds>& data);

        RigidBody* OwningBody = nullptr;
        rp3d::Collider* ColliderPtr = nullptr;
        rp3d::CollisionShape* Shape = nullptr;
        ShapeName ShapeName = BOX;
        bool DrawableBounds = false;

        Bounds bounds;

        void SetBounds(const Bounds& data);

        template <class Archive>
        void serialize(Archive& ar) {
            ar(cereal::make_nvp("CollisionShape", ShapeName));

            ar(cereal::make_nvp("Bounds", bounds));
        }
    };

    Collider::Bounds GetBounds(const Drawable& model, const Transform& transform);

    struct Terrain
    {
        std::shared_ptr<Controller::Terrain> terrain_ptr;

        std::string name;
        std::string path;
        float min, max;

        template <class Archive>
        void serialize(Archive& ar) {
            ar(cereal::make_nvp("Name", name));
            ar(cereal::make_nvp("Path", path));
            ar(cereal::make_nvp("Min", min));
            ar(cereal::make_nvp("Max", max));
        }
    };
}  // namespace Vakol::Model::Components

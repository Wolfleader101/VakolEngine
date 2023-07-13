#pragma once

#include <reactphysics3d/reactphysics3d.h>

#include <Controller/Physics/ScenePhysics.hpp>
#include <crossguid/guid.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "Controller/Animator.hpp"
#include "Controller/SolSerialize.hpp"
#include "Entity.hpp"
#include "Model/Assets/Model.hpp"
#include "ScriptEngine/ScriptEngine.hpp"

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

               cereal::make_nvp("rot.x", eulerAngles.x), cereal::make_nvp("rot.y", eulerAngles.y),
               cereal::make_nvp("rot.z", eulerAngles.z),

               cereal::make_nvp("scale.x", scale.x), cereal::make_nvp("scale.y", scale.y),
               cereal::make_nvp("scale.z", scale.z));
        }
    };

    /**
     * @brief Animator component
     */
    struct Animator {
        std::string attached_model; /**< The attached model name. */

        /**
         * \brief Updates the animator with the specified state and delta time.
         * \param state The state of the animator.
         * \param delta_time The elapsed time since the last update.
         */
        void Update(const int state, const float delta_time) { animator_ptr->Update(state, delta_time); }

        /**
         * \brief Updates the animator with the specified delta time.
         * \param delta_time The elapsed time since the last update.
         */
        void Update(const float delta_time) { animator_ptr->Update(delta_time); }

        [[nodiscard]] int nAnimations() const { return animator_ptr->nAnimations(); }

        /**
         * \brief Retrieves a constant reference to the animation for the specified state.
         * \param state The state of the animation.
         * \return A constant reference to the animation.
         */
        const Model::Assets::Animation& c_animation(const int state) const { return animator_ptr->c_get(state); }

        /**
         * \brief Retrieves a copy of the animation for the specified state.
         * \param state The state of the animation.
         * \return A copy of the animation.
         */
        Model::Assets::Animation animation(const int state) const { return animator_ptr->get(state); }

        /**
         * \brief Sets the animator using a shared pointer.
         * \param animator The shared pointer to the animator.
         */
        void set(const std::shared_ptr<Controller::Animator>& animator) { animator_ptr = animator; }

        /**
         * \brief Sets the animator using a non-const reference.
         * \param animator The animator object.
         */
        void set(const Controller::Animator& animator) {
            animator_ptr = std::make_shared<Controller::Animator>(animator);
        }

        /**
         * \brief Serializes the Animator object.
         * \tparam Archive The archive type.
         * \param ar The archive.
         */
        template <class Archive>
        void serialize(Archive& ar) {
            ar(cereal::make_nvp("attached_model", attached_model));
            // ar(cereal::make_nvp("State Table",state));
        }

        std::shared_ptr<Controller::Animator> animator_ptr = nullptr; /**< The pointer to the animator object. */
    };

    /**
     * @struct Animation
     * @brief Represents an animation.
     */
    struct Animation {
        int state = 0;              /**< The state of the animation. */
        std::string attached_model; /**< The attached model name. */

        /**
         * @brief Serializes the Animation object.
         * @tparam Archive The archive type.
         * @param ar The archive.
         */
        template <class Archive>
        void serialize(Archive& ar) {
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
    struct Tag {
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
     * @brief Script component that holds reference to file
     */
    struct Script {
        std::string script_name; /**< The name of the script file. */
        // sol::table state;              /**< The Lua state table. */
        Controller::SolTableData data; /**< The serialized Lua state table data. */

        /**
         * @brief Default constructor for Script.
         */
        Script() = default;

        /**
         * @brief Constructs a Script object with the specified script name.
         * @param name The name of the script.
         */
        explicit Script(std::string& name);

        /**
         * @brief Constructs a Script object with the specified script, Lua state, entity, and scene.
         * @param script The script file.
         * @param lua The shared pointer to the Lua state.
         * @param entity The entity.
         * @param scene The scene.
         */
        Script(const std::string& script, Entity& entity, Controller::Scene& scene);

        /**
         * @brief Serializes the Script object for saving.
         * @tparam Archive The archive type.
         * @param ar The archive.
         */
        template <class Archive>
        void save(Archive& ar) const {
            ar(cereal::make_nvp("ScriptName", script_name));

            // Controller::SolTableData temp;
            // Controller::ConvertSolToMap(state, temp);

            // ar(temp);
        }

        /**
         * @brief Deserializes the Script object for loading.
         * @tparam Archive The archive type.
         * @param ar The archive.
         */
        template <class Archive>
        void load(Archive& ar) {
            ar(cereal::make_nvp("ScriptName", script_name));

            data.data.clear();
            ar(data);
        }
    };

    /**
     * @brief Struct representing a finite state machine that can be controlled in lua.
     */
    struct FSM {
        /**
         * @brief Default constructor for the FSM struct.
         */
        FSM() = default;

        // TODO this is fucked
        FSM(LuaTable table);

        // The current state of the FSM.
        std::string currentState;

        // table of FSM states
        LuaTable states;

        // Controller::SolTableData data;  ///< The data to be saved or loaded.

        /**
         * @brief Adds a new state to the FSM.
         * @param stateName The name of the new state.
         * @param callback The function to be executed when this state is active.
         */
        void AddState(const std::string& stateName, LuaFunction& callback);

        /**
         * @brief Changes the current state of the FSM.
         * @param stateName The name of the state to change to.
         */
        void ChangeState(const std::string& stateName);

        /**
         * @brief Returns the current state of the FSM.
         * @return A string representing the current state.
         */
        std::string GetState();

        /**
         * @brief Updates the state of the FSM.
         */
        void Update();

        /**
         * @brief Saves the current state of the FSM.
         * @param ar Archive to save the data to.
         */
        template <class Archive>
        void save(Archive& ar) const {
            // ar(cereal::make_nvp("Current State", currentState));

            // Controller::SolTableData temp;
            // Controller::ConvertSolToMap(states, temp);

            // ar(temp);
        }

        /**
         * @brief Loads the state of the FSM from an archive.
         * @param ar Archive to load the data from.
         */
        template <class Archive>
        void load(Archive& ar) {
            // ar(cereal::make_nvp("Current State", currentState));

            // data.data.clear();
            // ar(data);
        }
    };

    /**
     * @brief Struct representing a drawable entity.
     */
    struct Drawable {
        /**
         * @brief Default constructor for the Drawable struct.
         */
        Drawable() = default;

        /**
         * @brief Overloaded constructor that initializes a Drawable with a file path.
         * @param file The file path to the drawable entity.
         */
        explicit Drawable(std::string&& file);

        /**
         * @brief Overloaded constructor that initializes a Drawable with detailed information.
         * @param file The file path to the drawable entity.
         * @param scale The scale of the drawable entity.
         * @param animated Boolean representing if the entity is animated.
         * @param backfaceCull Boolean representing if backface culling is enabled.
         */
        Drawable(const std::string& file, float scale, bool animated, bool backfaceCull);

        std::string name;  ///< Name of the drawable entity. Used for serialization.

        float scale = 1.0f;        ///< The scale of the drawable entity.
        bool animated = false;     ///< Boolean indicating if the entity is animated.
        bool backfaceCull = true;  ///< Boolean indicating if backface culling is enabled.
        bool instance = false;     ///< Boolean indicating if the entity is an instance.
        bool active = true;        ///< Boolean indicating if the entity is active.

        std::shared_ptr<Assets::Model> model_ptr;  ///< Shared pointer to the model of the entity.

        /**
         * @brief Serializes the drawable entity.
         * @param ar Archive to serialize the data to.
         */
        template <class Archive>
        void serialize(Archive& ar) {
            ar(cereal::make_nvp("Model", name));
            ar(cereal::make_nvp("Import Scale", scale));
            ar(cereal::make_nvp("Animated", animated));
            ar(cereal::make_nvp("Back Face Culling", backfaceCull));
        }
    };

    using namespace Vakol::Controller::Physics;

    /**
     * @brief Struct representing a RigidBody.
     */
    struct RigidBody {
        /**
         * @brief Enum for the type of body.
         */
        enum BODY_TYPE {
            STATIC = rp3d::BodyType::STATIC,        ///< Represents a static body type.
            KINEMATIC = rp3d::BodyType::KINEMATIC,  ///< Represents a kinematic body type.
            DYNAMIC = rp3d::BodyType::DYNAMIC       ///< Represents a dynamic body type.
        };

        /**
         * @brief Default constructor for the RigidBody struct.
         */
        RigidBody() = default;

        bool initialized = false;
        bool use_transform = false;
        // bool use_y_pos = false;
        bool is_colliding = false;
        bool was_colliding = false;

        /**
         * @brief Struct representing the data for a rigid body.
         */
        struct RigidData {
            float mass = 3;                         ///< Mass of the rigid body.
            bool grav = false;                      ///< Boolean indicating if gravity is enabled on the rigid body.
            float LDamp = 0;                        ///< Linear dampening factor.
            float ADamp = 1;                        ///< Angular dampening factor.
            rp3d::Vector3 AngularLock = {0, 1, 0};  ///< Angular lock axis factor.
            rp3d::Vector3 Orientation = {0, 0, 0};  ///< Orientation of the rigid body.
        };

        /**
         * @brief Sets the rigid data for the body.
         * @param data The rigid data to be set.
         */
        void SetRigidData(const RigidData& data);
        void ToggleGravity();  ///< Toggles gravity for the rigid body.
        /**
         * @brief Sets the body type for the rigid body.
         * @param t The body type to be set.
         */
        void SetBodyType(BODY_TYPE t);
        /**
         * @brief Sets the velocity for the rigid body.
         * @param vel The velocity to be set.
         */
        void SetVelocity(const glm::vec3& vel) const;
        /**
         * @brief Sets the angular velocity for the rigid body.
         * @param vel The angular velocity to be set.
         */
        void SetAngularVelocity(const glm::vec3& vel) const;
        /**
         * @brief Sets the linear dampening for the rigid body.
         * @param damp The linear dampening factor to be set.
         */
        void SetLinearDamp(float damp) const;
        /**
         * @brief Sets the angular dampening for the rigid body.
         * @param damp The angular dampening factor to be set.
         */
        void SetAngularDamp(float damp) const;
        void ApplyForce(const glm::vec3& force) const;

        std::shared_ptr<ScenePhysics> owningWorld = nullptr;  ///< Shared pointer to the world owning the rigid body.
        rp3d::RigidBody* RigidBodyPtr = nullptr;              ///< Pointer to the rigid body.
        BODY_TYPE Type = DYNAMIC;                             ///< The type of the rigid body. Default is DYNAMIC.
        RigidData Data;                                       ///< The rigid data for the rigid body.

        rp3d::Transform prevTransform;  ///< The previous transform of the rigid body.

        /**
         * @brief Serializes the rigid body.
         * @param ar Archive to serialize the data to.
         */
        template <class Archive>
        void serialize(Archive& ar) {
            ar(cereal::make_nvp("Mass", Data.mass));
            ar(cereal::make_nvp("Gravity", Data.grav));
            ar(cereal::make_nvp("Linear Dampening", Data.LDamp));
            ar(cereal::make_nvp("Angular Dampening", Data.ADamp));
            ar(cereal::make_nvp("BodyType", Type));
            ar(cereal::make_nvp("Use Transform", use_transform));

            ar(cereal::make_nvp("Angular Lock X", Data.AngularLock.x));
            ar(cereal::make_nvp("Angular Lock Y", Data.AngularLock.y));
            ar(cereal::make_nvp("Angular Lock Z", Data.AngularLock.z));

            ar(cereal::make_nvp("Orientation X", Data.Orientation.x));
            ar(cereal::make_nvp("Orientation Y", Data.Orientation.y));
            ar(cereal::make_nvp("Orientation Z", Data.Orientation.z));
        }
    };

    /**
     * @brief Struct representing a Collider.
     */
    struct Collider {
        /**
         * @brief Enum for the name of the shape of the collider.
         */
        enum ShapeName { BOX, SPHERE, CAPSULE, TRIANGLE_MESH };

        /**
         * @brief Struct representing the bounds of a collider.
         */
        struct Bounds {
            rp3d::Vector3 min = {0, 0, 0};              /**< minimum vertice*/
            rp3d::Vector3 max = {1, 1, 1};              /**< Maximum vertice*/
            rp3d::Vector3 center = {0.5f, 0.5f, 0.5f};  /**< Average of all vertices*/
            rp3d::Vector3 extents = {0.5f, 0.5f, 0.5f}; /**< Extent of vertices*/
            rp3d::Vector3 size = {1, 1, 1};             /**< Size of vertices*/

            float radius = 0.5f * extents.length(); /**< Radius*/

            /**
             * @brief Serializes the bounds of the collider.
             * @param ar Archive to serialize the data to.
             */
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

        /**
         * @brief Default constructor for the Collider struct.
         */
        Collider() = default;

        /**
         * @brief Overloaded constructor that initializes a Collider with the owner and the bounds.
         * @param owner The owner of the collider.
         * @param data The bounds of the collider.
         */
        Collider(RigidBody& owner, const std::optional<Bounds>& data);

        RigidBody* OwningBody = nullptr;        ///< Pointer to the body owning the collider.
        rp3d::Collider* ColliderPtr = nullptr;  ///< Pointer to the collider.
        rp3d::CollisionShape* Shape = nullptr;  ///< Pointer to the shape of the collider.
        ShapeName ShapeName = BOX;              ///< The name of the shape of the collider. Default is BOX.
        bool DrawableBounds = false;            ///< Boolean indicating if the bounds are drawable.

        Bounds bounds;  ///< The bounds of the collider.

        /**
         * @brief Sets the bounds for the collider.
         * @param data The bounds to be set.
         */
        void SetBounds(const Bounds& data);

        /**
         * @brief Serializes the collider.
         * @param ar Archive to serialize the data to.
         */
        template <class Archive>
        void serialize(Archive& ar) {
            ar(cereal::make_nvp("CollisionShape", ShapeName));

            ar(cereal::make_nvp("Bounds", bounds));
        }
    };

    /**
     * @brief Function to get the bounds of a collider.
     * @param model Reference to the model of the Drawable.
     * @param transform Reference to the Transform.
     * @return Bounds of the collider.
     */
    Collider::Bounds GetBounds(const Drawable& model, const Transform& transform);

    /**
     * @brief Struct representing a Terrain.
     */
    struct Terrain {
        std::shared_ptr<Controller::Terrain> terrain_ptr;  ///< Shared pointer to the terrain.

        std::string name;  ///< Name of the terrain.
        std::string path;  ///< Path to the terrain.
        float min;         ///< Minimum value for the terrain.
        float max;         ///< Maximum value for the terrain.

        /**
         * @brief Serializes the terrain.
         * @param ar Archive to serialize the data to.
         */
        template <class Archive>
        void serialize(Archive& ar) {
            ar(cereal::make_nvp("Name", name));
            ar(cereal::make_nvp("Path", path));
            ar(cereal::make_nvp("Min", min));
            ar(cereal::make_nvp("Max", max));
        }
    };

    /**
     * @brief Struct representing a Globally Unique Identifier (GUID).
     */
    struct GUID {
        /**
         * @brief Default constructor for the GUID struct.
         */
        GUID() = default;

        /**
         * @brief Function to generate a new GUID.
         */
        void GenNewGUID();

        /**
         * @brief Equality operator for GUID.
         * @param other The other GUID to compare with.
         * @return True if the GUIDs are equal, false otherwise.
         */
        bool operator==(const GUID& other) const;

        /**
         * @brief Inequality operator for GUID.
         * @param other The other GUID to compare with.
         * @return True if the GUIDs are not equal, false otherwise.
         */
        bool operator!=(const GUID& other) const;

        /**
         * @brief Less than operator for GUID.
         * @param other The other GUID to compare with.
         * @return True if the current GUID is less than the other GUID, false otherwise.
         */
        bool operator<(const GUID& other) const;

        xg::Guid id;  ///< The actual GUID.

        /**
         * @brief Function to save the GUID.
         * @param ar Archive to save the GUID to.
         */
        template <class Archive>
        void save(Archive& ar) const {
            std::string id_str = id.str();
            ar(cereal::make_nvp("guid", id_str));
        }

        /**
         * @brief Function to load the GUID.
         * @param ar Archive to load the GUID from.
         */
        template <class Archive>
        void load(Archive& ar) {
            std::string id_str;
            ar(cereal::make_nvp("guid", id_str));
            id = xg::Guid(id_str);
        }
    };

}  // namespace Vakol::Model::Components

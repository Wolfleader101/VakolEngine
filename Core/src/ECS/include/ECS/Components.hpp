#pragma once

#include <memory>
#include <string>

#include "Entity.hpp"
#include "Math/Math.hpp"
#include "Rendering/Assets/Model.hpp"
#include "Scripting/ScriptTypes.hpp"
#include "Serialisation/SolSerialize.hpp"

namespace Vakol::Components
{
    enum LIGHT_TYPE
    {
        DIRECTIONAL_LIGHT,
        SPOT_LIGHT,
        POINT_LIGHT
    };

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
        Transform(const Math::Vec3& pos, const Math::Quat& rot, const Math::Vec3& scale);

        Math::Vec3 pos = Math::Vec3(0.0f, 0.0f, 0.0f); /**< XYZ position */

        Math::Vec3 eulerAngles = Math::Vec3(0.0f, 0.0f, 0.0f);

        Math::Quat rot = Math::Quat(1.0f, 0.0f, 0.0f, 0.0f); /**< WXYZ rotation */

        Math::Vec3 scale = Math::Vec3(1.0f, 1.0f, 1.0f); /**< XYZ scale */

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(cereal::make_nvp("pos.x", pos.x), cereal::make_nvp("pos.y", pos.y), cereal::make_nvp("pos.z", pos.z),

               cereal::make_nvp("rot.x", eulerAngles.x), cereal::make_nvp("rot.y", eulerAngles.y),
               cereal::make_nvp("rot.z", eulerAngles.z),

               cereal::make_nvp("scale.x", scale.x), cereal::make_nvp("scale.y", scale.y),
               cereal::make_nvp("scale.z", scale.z));
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
        bool IsEmpty() const
        {
            return tag.empty();
        }

        Tag() = default;
        explicit Tag(std::string&);

        std::string tag; /**< String object containing data*/

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(cereal::make_nvp("tag", tag));
        }
    };

    /**
     * @enum ENTITY_TYPE
     *
     * @brief enum value indicating the type of entity it is
     */
    enum ENTITY_TYPE
    {
        UNKNOWN,
        WORLD,
        PLAYER,
        ENEMY,
        FRIEND,
        COLLECTABLE,
        EXIT
    };

    /**
     *
     * @struct TagType
     *
     * @brief struct containing EntityType enum indicating the type of entity.
     *
     */
    struct TagType
    {
        ENTITY_TYPE type; /**< Type of entity */

        TagType() = default;
        explicit TagType(uint8_t);

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(cereal::make_nvp("TagType", type));
        }
    };

    /**
     * \brief A struct containing information about a light
     */
    struct Light
    {
        unsigned int LIGHT_TYPE = DIRECTIONAL_LIGHT;

        const float LIGHT_ATTENUATION_CONSTANT = 1.0f;
        float LIGHT_ATTENUATION_LINEAR = 0.045f;
        float LIGHT_ATTENUATION_QUADRATIC = 0.0075f;

        float LIGHT_ATTENUATION_CUTOFF = Math::Cos(Math::DegToRad(7.5f));
        float LIGHT_ATTENUATION_OUTER_CUTOFF = Math::Cos(Math::DegToRad(12.5f));

        Math::Vec3 LIGHT_DIRECTION = Math::Vec3(0.0f);
        Math::Vec3 LIGHT_POSITION = Math::Vec3(0.0f);
    };

    /**
     * @brief Struct representing a finite state machine that can be controlled in lua.
     */
    struct FSM
    {
        /**
         * @brief Default constructor for the FSM struct.
         */
        FSM() = default;

        // TODO probably dont want to store a table in FSM
        FSM(LuaTable table);

        // The current state of the FSM.
        std::string currentState;

        // table of FSM states
        LuaTable states;

        // SolTableData data;  ///< The data to be saved or loaded.

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
        void save(Archive& ar) const
        {
            // ar(cereal::make_nvp("Current State", currentState));

            // SolTableData temp;
            // ConvertSolToMap(states, temp);

            // ar(temp);
        }

        /**
         * @brief Loads the state of the FSM from an archive.
         * @param ar Archive to load the data from.
         */
        template <class Archive>
        void load(Archive& ar)
        {
            // ar(cereal::make_nvp("Current State", currentState));

            // data.data.clear();
            // ar(data);
        }
    };

} // namespace Vakol::Components

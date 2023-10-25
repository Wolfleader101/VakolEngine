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

        Math::Vec4 LIGHT_DIRECTION = Math::Vec4(0.0f);
        Math::Vec4 LIGHT_POSITION = Math::Vec4(0.0f);
    };

} // namespace Vakol::Components

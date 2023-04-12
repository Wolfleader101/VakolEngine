#pragma once

#include <reactphysics3d/reactphysics3d.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <memory>
#include <string>

#include "Controller/LuaState.hpp"
#include "Entity.hpp"
#include "Model/Assets/Model.hpp"

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
        glm::vec3 scale; /**< XYZ scale */

        template <class Archive>
        void serialize(Archive& ar) {
            ar(cereal::make_nvp("pos.x", pos.x), cereal::make_nvp("pos.y", pos.y), cereal::make_nvp("pos.z", pos.z),

               cereal::make_nvp("rot.w", rot.w), cereal::make_nvp("rot.x", rot.x), cereal::make_nvp("rot.y", rot.y),
               cereal::make_nvp("rot.z", rot.z),

               cereal::make_nvp("scale.x", scale.x), cereal::make_nvp("scale.y", scale.y),
               cereal::make_nvp("scale.z", scale.z));
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

        Script(const std::string& script, Controller::LuaState& lua, Model::Entity& entity);

        template <class Archive>
        void serialize(Archive& ar) {
            ar(cereal::make_nvp("ScriptName", script_name));
        }
    };

    struct Drawable {
        std::shared_ptr<Vakol::Model::Assets::Model> model_ptr;
    };

}  // namespace Vakol::Model::Components

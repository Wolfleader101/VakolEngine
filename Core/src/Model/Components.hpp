#pragma once

#include <reactphysics3d/reactphysics3d.h>

#include <Controller/LuaState.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>

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

        std::string tag; /**< String object containing data*/
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
    };

    /**
     * @struct Script
     *
     * @brief script component that holds reference to file
     */
    struct Script {
        std::string script_name;

        Script(const std::string& script, Controller::LuaState& lua);
    };

}  // namespace Vakol::Model::Components

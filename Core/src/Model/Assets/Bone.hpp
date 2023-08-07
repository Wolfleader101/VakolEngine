#pragma once

#include <map>
#include <string>

#include <Controller/Logger.hpp>

#include <glm/mat4x4.hpp>

namespace Vakol::Model::Assets
{
    /**
     * @brief Struct representing a bone in an animation.
     */
    struct Bone
    {
        int index = -1;   /**< The index of the bone. */
        glm::mat4 offset; /**< The inverse-bind matrix or offset matrix of the bone. */
    };

    /**
     * @brief Struct representing a map of bone information.
     */
    struct BoneMap
    {
        std::map<std::string, Bone, std::less<>>
            name_to_info;     /**< The map to store bone information, using bone names as keys. */
        int next_bone_id = 0; /**< The next available bone ID for adding new bones. */

        /**
         * @brief Add a new bone to the map with the given name and bone-to-model transformation matrix.
         *
         * @param name The name of the bone.
         * @param bone_to_model The bone-to-model transformation matrix.
         * @return The index of the added bone.
         */
        int add_new_bone(std::string &&name, const glm::mat4 &bone_to_model)
        {
            // Insert the new bone into the map
            auto [itr, inserted] = name_to_info.insert(std::make_pair(std::move(name), Bone{-1, bone_to_model}));

            if (!inserted)
                return itr->second.index;

            // Retrieve the inserted bone's index and set it to the next available bone ID
            auto &[index, offset] = itr->second;
            index = next_bone_id++;

            return index;
        }

        /**
         * @brief Get the Bone pointer for the bone with the given name.
         *
         * @param name The name of the bone.
         * @return The pointer to the Bone if the bone is found, nullptr otherwise.
         */
        const Bone *get(const char *name) const
        {
            // Find the bone in the map based on the name
            const auto itr = name_to_info.find(name);

            if (itr == name_to_info.end())
                VK_WARN("Unable to find bone with name: {0}", name);

            // Return the Bone pointer if the bone is found, otherwise return nullptr
            return itr != name_to_info.end() ? &itr->second : nullptr;
        }
    };
} // namespace Vakol::Model::Assets
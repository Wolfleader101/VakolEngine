#pragma once

#include <map>
#include <string>

#include <Controller/Logger.hpp>

namespace Vakol::Model::Assets
{
	struct Bone
	{
		int index = -1; // Index of the bone

		// Inverse-bind matrix or offset matrix
		glm::mat4 offset;
	};

	struct BoneMap
	{
		std::map<std::string, Bone, std::less<>> name_to_info; // Map to store bone information, using bone names as keys

		int next_bone_id = 0; // Next available bone ID for adding new bones

		// Add a new bone to the map with the given name and bone-to-model transformation matrix
		int add_new_bone(std::string&& name, const glm::mat4& bone_to_model)
		{
			// Insert the new bone into the map
			auto [itr, inserted] = name_to_info.insert(std::make_pair(std::move(name), Bone{ -1, bone_to_model }));

			if (!inserted)
				return itr->second.index;

			// Retrieve the inserted bone's index and set it to the next available bone ID
			auto& [index, offset] = itr->second;
			index = next_bone_id++;

			return index;
		}

		// Get the Bone pointer for the bone with the given name
		const Bone* get(const char* name) const
		{
			// Find the bone in the map based on the name
			const auto itr = name_to_info.find(name);

			if (itr == name_to_info.end()) VK_CRITICAL("Unable to find bone with name: {0}", name);

			// Return the BoneInfo pointer if the bone is found, otherwise return nullptr
			return itr != name_to_info.end() ? &itr->second : nullptr;
		}
	};
}
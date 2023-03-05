#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <reactphysics3d/reactphysics3d.h>

#include <string>

#include <Model/Mesh.hpp> //don't know why this needs src but sure
#include <Model/Material.hpp>
#include <Model/Shader.hpp>
#include <Model/Model.hpp>

namespace Vakol::Model::ECS::Components
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
			* @param sca scale
			*/
		Transform(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scale);


		glm::vec3 pos;		/**< XYZ position */
		glm::quat rot;		/**< XYZ rotation */
		glm::vec3 scale;    /**< XYZ scale */
	};

	using ModelType = Model;

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
		bool IsEmpty()
		{
			return tag.size() == 0;
		}

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
	struct TagType
	{


		EntityType type; /**< Type of entity */
	};
		

	
}

#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <reactphysics3d/reactphysics3d.h>

#include <string>

#include <Resources/Mesh/Mesh.hpp>
#include <Resources/Material/Material.hpp>
#include <Renderer/Shader/Shader.hpp>
#include <Resources/Model/Model.hpp>

namespace Vakol
{
	/**
	 * @namespace Components
	 *
	 *
	 * @brief Namespace added to indicate where components are used
	 *
	 *
	 */
	namespace Components
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
		 * @struct RigidBody
		 * @brief the rigidbody component for everything physics related
		 *
		 */
		struct RigidBody
		{
			struct RigidData
			{
				double mass;			/**< Mass of object*/
				bool grav;				/**< If gravity is enabled on the object*/
				double LDamp;			/**< Linear Dampening*/
				double ADamp;			/**< Angular Dampening*/
				rp3d::Vector3 AngularLock; /**< Angular lock axis factor */
				rp3d::Vector3 Orientation;
				
			};

			RigidData Data;					/**< Data of the Object we need for physics*/

			/**
			 * @brief rp3d rigidbody pointer
			 */
			rp3d::RigidBody* body;

			/**
			 * @brief rp3d body type
			 */
			rp3d::BodyType type; /**< Object type. See here -> 0 = static | 1 = kinematic | 2 = dynamic */

			/**
			 * @brief previous transform of rigidbody
			 */
			rp3d::Transform prevTransform;
		};


			/**
			 * @struct Collider
			 * 
			 * @brief stores pointer to the collider and its shape type
			 * 
			 */
		struct Collider
		{
			/**
			 * @struct Bounds
			 * 
			 * @brief Data in relation to the mesh given. Used to give appropriate sizing to collider
			 * 
			 */
			struct Bounds
			{
				rp3d::Vector3 min; /**< minimum vertice*/
				rp3d::Vector3 max; /**< Maximum vertice*/
				rp3d::Vector3 center; /**< Average of all vertices*/
				rp3d::Vector3 extents; /**< Extent of vertices*/
				rp3d::Vector3 size; /**< Size of vertices*/

				float radius; /**< Radius*/
			};

			rp3d::Vector3 offset = {0.0f, 0.0f, 0.0f}; /**< Offset given to the collider*/

			rp3d::Collider* collider; /**< Pointer to actual collider in react physics*/
			rp3d::CollisionShape* shape; /**< pointer to collider shape*/
			rp3d::CollisionShapeName shapeType; /**< Type of shape the collider is. cube/sphere/mesh/capsule etc...*/

			Mesh mesh; /**< Mesh collider is referenced from*/
			Bounds bounds; /**< Data deducted from mesh*/

			/**
			 * @brief Get bounds of mesh data
			 * 
			 * @param msh mesh to get data from
			 * @param bounds bounds variable to fill
			 */
			void GetBounds(const Mesh& msh, Bounds& bounds); 
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
		enum EntityType { UNKNOWN, WORLD, PLAYER, ENEMY, FRIEND, COLLECTABLE, EXIT, /* Adding but to be removed ->*/ GOLDCOIN, SILVERCOIN, BRONZECOIN, DJUMP, SLOW, FAST };
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




}

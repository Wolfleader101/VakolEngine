#pragma once

#include <entt/entt.hpp>

#include <list>


namespace Vakol
{

	class Entity; //pre declared to prevent recursive include

	/**
	 * @class EntityList
	 *
	 *
	 * @brief Wrapper for entt registry and contains a list of all active entities
	 *
	*/
	class EntityList
	{
	public:

		/**
		 * @brief Construct a new Entity Manager object
		 *
		 */
		EntityList() = default;

		/**
		 * @brief Create an Entity object
		 *
		 * @return Entity created
		 */
		Entity& CreateEntity();

		/**
		 * @brief Create an Entity object
		 *
		 * @param SuggestedHandle handle to use for entity
		 * @return Entity created
		 */
		Entity CreateEntity(uint32_t SuggestedHandle);

		/**
		 * @brief Get an Entity object if it exists
		 *
		 * @param Handle to get
		 * @return Entity from list
		 */
		Entity GetEntity(uint32_t Handle);

		/**
		 * @brief Removes entity if it exists
		 *
		 * @param Handle to remove
		 */
		void RemoveEntity(uint32_t Handle); //can also use entity because type conversion provided in entity

		/**
		 * @brief clears list and registry of all entities
		 *
		*/
		void Clear();

			/**
			 * @brief Checks registry for existing entity handle
			 *
			 */
		bool CheckEntityExistence(uint32_t Handle) const;

			/**
			 * @brief Get reference to the Entity List object
			 *
			 * @return std::list<Entity>&
			 */
		std::list<Entity>& GetEntityList();



	private:
		/**
		 * @brief EnTT registry actually containing all the entity and comp data
		*/
		entt::registry m_Registry; 

		/**
		 * @brief list of entities created and active
		*/
		std::list<Entity> ActiveEntityList;

		friend class Entity; //friend to allow the api to entities to be clean.
	};
}
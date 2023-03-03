#include "EntityList.hpp"

#include "Entity.hpp"
#include "Components.hpp"

#include <Logger/Logger.hpp>


#include <assert.h>

namespace Vakol
{

	Entity& EntityList::CreateEntity()
	{
		Entity newEntity = Entity(m_Registry.create(), this);

		ActiveEntityList.push_back(newEntity);


		return ActiveEntityList.back();
	}

	Entity EntityList::CreateEntity(uint32_t SuggestedHandle)
	{
		Entity newEntity = Entity(m_Registry.create((entt::entity)SuggestedHandle), this);

		ActiveEntityList.push_back(newEntity);

		return newEntity;

	}


	Entity EntityList::GetEntity(uint32_t Handle)
	{
		if (!CheckEntityExistence(Handle)) //ensuring the entity actually exists
		{
			VK_CRITICAL("EntityList.GetEntity(uint32_t): Entity with given handle does not exist");
			assert(0);
		}
		Entity newEnt;

		newEnt.m_entityHandle = (entt::entity)Handle; newEnt.m_EntityList = this;
		return newEnt;
	}

	void EntityList::RemoveEntity(uint32_t Handle)
	{
		entt::entity ID = (entt::entity)Handle;

		if (!CheckEntityExistence(Handle)) //ensuring the entity actually exists
		{
			VK_ERROR("EntityList.RemoveEntity(uint32_t): Entity with given handle does not exist... skipping");
		}
		else
		{
			m_Registry.destroy(ID);

			auto it = ActiveEntityList.begin();
			auto end = ActiveEntityList.end();

			while (it != end)
			{
				if (it->GetHandle() == Handle)
				{
					ActiveEntityList.erase(it);
					break;
				}
				it++;
			}
		}

		
	}

	void EntityList::Clear()
	{
		m_Registry.clear();
		ActiveEntityList.clear();
	}

	bool EntityList::CheckEntityExistence(uint32_t Handle) const
	{
		return m_Registry.valid((entt::entity)Handle);
	}

	std::list<Entity>& EntityList::GetEntityList()
	{
		return ActiveEntityList;
	}
}
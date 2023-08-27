#include "ECS/EntityList.hpp"

#include <cassert>
#include <cereal/cereal.hpp>

#include "ECS/Components.hpp"
#include "ECS/Entity.hpp"
#include "Logger/Logger.hpp"

using namespace Vakol;

namespace Vakol
{

    EntityList::EntityList() : m_Registry(), ActiveEntityList(){};

    Entity EntityList::CreateEntity()
    {
        Entity newEntity = Entity(m_Registry.create(), this);

        ActiveEntityList.push_back(newEntity);

        return ActiveEntityList.back();
    }

    Entity EntityList::CreateEntity(uint32_t SuggestedHandle)
    {
        Entity newEntity = Entity(m_Registry.create(static_cast<entt::entity>(SuggestedHandle)), this);

        ActiveEntityList.push_back(newEntity);

        return newEntity;
    }

    Entity EntityList::GetEntity(uint32_t Handle)
    {
        if (!CheckEntityExistence(Handle)) // ensuring the entity actually exists
        {
            VK_CRITICAL("EntityList.GetEntity(uint32_t): Entity with given handle does not exist");
            assert(0);
        }
        Entity newEnt;

        newEnt.m_entityHandle = static_cast<entt::entity>(Handle);
        newEnt.m_EntityList = this;
        return newEnt;
    }

    void EntityList::RemoveEntity(uint32_t Handle)
    {
        entt::entity ID = static_cast<entt::entity>(Handle);

        if (!CheckEntityExistence(Handle)) // ensuring the entity actually exists
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
        return m_Registry.valid(static_cast<entt::entity>(Handle));
    }

    std::vector<Entity>& EntityList::GetEntityVec()
    {
        return ActiveEntityList;
    }

    //----------------------- ICT397 additions

    void EntityList::Serialize(const std::string& file) const
    {
        privateSerialize<cereal::JSONOutputArchive, Components::Transform, Components::Tag, Components::GUID>(file);

        /*privateSerialize<cereal::JSONOutputArchive, Transform, Tag, Script, GUID, Drawable, Components::AnimatorComp,
                         Animation>(file);*/
    }

    void EntityList::Deserialize(const std::string& file)
    {
        privateDeserialize<cereal::JSONInputArchive, Components::Transform, Components::Tag, Components::GUID>(file);

        /*privateDeserialize<cereal::JSONInputArchive, Transform, Tag, Script, GUID, Drawable, Components::AnimatorComp,
                           Animation>(file);*/
    }

} // namespace Vakol

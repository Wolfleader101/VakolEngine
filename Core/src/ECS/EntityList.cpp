#include "include/EntityList.hpp"

#include <assert.h>
#include <cereal/cereal.hpp>

#include "ECS/include/Entity.hpp"
#include "Logger/include/Logger.hpp"
#include "Model/Components.hpp"
#include "include/System.hpp"

using namespace Vakol::Model;

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
        Entity newEntity = Entity(m_Registry.create((entt::entity)SuggestedHandle), this);

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

        newEnt.m_entityHandle = (entt::entity)Handle;
        newEnt.m_EntityList = this;
        return newEnt;
    }

    void EntityList::RemoveEntity(uint32_t Handle)
    {
        entt::entity ID = (entt::entity)Handle;

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
        return m_Registry.valid((entt::entity)Handle);
    }

    std::vector<Entity>& EntityList::GetEntityVec()
    {
        return ActiveEntityList;
    }

    //----------------------- ICT397 additions

    void EntityList::Serialize(const std::string& file) const
    {
        privateSerialize<cereal::JSONOutputArchive, Components::Transform, Components::Tag, Components::GUID,
                         Components::Drawable, Components::AnimatorComp, Components::Animation, Components::RigidBody,
                         Components::Collider, Components::TerrainComp>(file);

        /*privateSerialize<cereal::JSONOutputArchive, Transform, Tag, Script, GUID, Drawable, Components::AnimatorComp,
                         Animation>(file);*/
    }

    void EntityList::Deserialize(const std::string& file)
    {
        privateDeserialize<cereal::JSONInputArchive, Components::Transform, Components::Tag, Components::GUID,
                           Components::Drawable, Components::AnimatorComp, Components::Animation, Components::RigidBody,
                           Components::Collider, Components::TerrainComp>(file);

        /*privateDeserialize<cereal::JSONInputArchive, Transform, Tag, Script, GUID, Drawable, Components::AnimatorComp,
                           Animation>(file);*/
    }

} // namespace Vakol
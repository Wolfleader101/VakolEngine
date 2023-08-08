#include "Entity.hpp"

#include <Model/Components.hpp>

namespace Vakol
{

    Entity::Entity(entt::entity handle, EntityList* EM) : m_entityHandle(handle), m_EntityList(EM)
    {

        this->AddComponent<Components::Tag>();

        this->AddComponent<Components::GUID>();

        this->AddComponent<Components::Transform>(); // every entity has a transform. Can change easily
    }

    uint32_t Entity::GetHandle() const
    {
        return uint32_t(m_entityHandle);
    }

    void Entity::SetHandle(uint32_t handle)
    {
        m_entityHandle = (entt::entity)handle;
    }

    Entity::operator uint32_t() const
    {
        return (uint32_t)m_entityHandle;
    }

    Entity::operator bool() const
    {
        return m_entityHandle != entt::null && m_EntityList != nullptr;
    }

} // namespace Vakol
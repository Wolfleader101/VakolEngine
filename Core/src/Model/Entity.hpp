#pragma once

#include <Controller/EntityList.hpp>
#include <Controller/Logger.hpp>
#include <entt/entt.hpp>
#include <cereal/archives/json.hpp>
#include <functional>
#include <string>

namespace Vakol::Model {

    /**
     * @class Entity
     *
     *
     * @brief Entity stores an ID and reference to the EntityList it is stored in
     * 		  These objects are used to modify an entity's component structure.
     *
     */
    class Entity {
       public:
        Entity() = default;

        /**
         * @brief Construct a new Entity object
         * 		  this is the intended constructor. Entity = Scene.CreateEntity()
         * @param handle entity ID/handle
         * @param scene to add entity too
         */
        Entity(entt::entity handle,
               Controller::EntityList* scene);  // this is the intended constructor. Entity = Scene.CreateEntity()

        /**
         * @brief copy constructor of a new entity
         * @param otherEntity to copy
         */
        Entity(const Entity& otherEntity) = default;  // may want this to actually copy over the components in the
                                                      // future

        /**
         * @brief returns true if all components listed are associated with entity
         *
         * @tparam Types - Variable number of types
         */
        template <typename... Types>
        bool HasComponent() const;

        /**
         * @brief returns true if any of the components listed are associated with entity
         *
         * @tparam Types - Variable number of types
         */
        template <typename... Types>
        bool HasAnyComponent() const;

        /**
         * @brief function to add a new component to entity if it already doesn't exist.
         *
         * @tparam T Component to be added
         * @param args construction arguments for type T
         */
        template <class T, typename... Args>
        void AddComponent(Args&&... args);

        /**
         * @brief Removes given component from entity if it exists
         *
         */
        template <class T>
        void RemoveComponent();

        /**
         * @brief Get the Component object
         *
         */
        template <class T>
        T& GetComponent() const;

        /**
         * @brief Get the Handle ID
         *
         * @return uint32_t handle of entity
         */
        uint32_t GetHandle() const;

        /**
         * @brief set the handle ID of entity
         * @param handle to set
         */
        void SetHandle(uint32_t handle);

        /**
         * @brief Cast for turning entity into just its handle. Used for function calls in EntityList
         *
         * @return uint32_t
         */
        operator uint32_t() const;

        /**
         * @brief Check for if an entity is valid
         *
         */
        operator bool() const;

        template<class Archive>
        void serialize(Archive& ar)
        {
            ar(cereal::make_nvp("handle", uint8_t(m_entityHandle)));
        }

       private:
        /**
         * @brief entity handle
         */
        entt::entity m_entityHandle{entt::null};

        /**
         * @brief entity list pointer
         */
        Controller::EntityList* m_EntityList = nullptr;

        friend Controller::EntityList;
    };

    template <typename... Types>
    inline bool Entity::HasComponent() const {
        return m_EntityList->m_Registry.all_of<Types...>(m_entityHandle);
    }

    template <typename... Types>
    inline bool Entity::HasAnyComponent() const {
        return m_EntityList->m_Registry.all_of<Types..>(m_entityHandle);
    }

    template <class T, typename... Args>
    inline void Entity::AddComponent(Args&&... args) {
        if (this->HasComponent<T>())
            VK_ERROR(
                "Entity.AddComponent<>(): Entity already has component... skipping");  // could make it that the new
                                                                                       // component over writes the old
        else
            m_EntityList->m_Registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
    }

    template <class T>
    inline void Entity::RemoveComponent() {
        if (!this->HasComponent<T>())
            VK_ERROR("Entity.RemoveComponent<>(): Entity does not have given component... skipping");  // error instead
                                                                                                       // of critical as
                                                                                                       // it wont crash
        else
            m_EntityList->m_Registry.remove<T>(m_entityHandle);
    }

    template <class T>
    inline T& Entity::GetComponent() const {
        if (!this->HasComponent<T>()) {
            VK_CRITICAL("Entity does not have given component!");
            assert(0);  // asserting because I need to return a type.
        }

        return m_EntityList->m_Registry.get<T>(m_entityHandle);
    }

}  // namespace Vakol::Model

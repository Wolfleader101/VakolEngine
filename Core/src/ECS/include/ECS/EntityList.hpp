#pragma once

#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <entt/entt.hpp>
#include <memory>
#include <set>
#include <vector>

#include "Logger/Logger.hpp"

namespace Vakol
{

    class Entity; // pre declared to prevent recursive include
    class System;
    class Scene;

    /**
     * @class EntityList
     *
     * @brief Wrapper for entt registry and contains a list of all active entities.
     */
    class EntityList
    {
      public:
        /**
         * @brief Construct a new Entity Manager object.
         */
        EntityList();

        /**
         * @brief Create an Entity object.
         *
         * @return Entity created.
         */
        Entity CreateEntity();

        /**
         * @brief Create an Entity object.
         *
         * @param SuggestedHandle Handle to use for entity.
         * @return Entity created.
         */
        Entity CreateEntity(uint32_t SuggestedHandle);

        /**
         * @brief Get an Entity object if it exists.
         *
         * @param Handle Handle to get.
         * @return Entity from list.
         */
        Entity GetEntity(uint32_t Handle);

        /**
         * @brief Removes an entity if it exists.
         *
         * @param Handle Handle to remove.
         */
        void RemoveEntity(uint32_t Handle); // can also use entity because type conversion is provided in entity

        /**
         * @brief Clears the list and registry of all entities.
         */
        void Clear();

        /**
         * @brief Checks the registry for the existence of an entity handle.
         *
         * @param Handle Handle to check.
         * @return bool True if the entity exists, false otherwise.
         */
        bool CheckEntityExistence(uint32_t Handle) const;

        /**
         * @brief Get a reference to the Entity List object.
         *
         * @return std::vector<Entity>& Reference to the entity vector.
         */
        std::vector<Entity>& GetEntityVec();

        /**
         * @brief Serialize the entity list to a file.
         *
         * @param file The file path to serialize to.
         */
        void Serialize(const std::string& file) const;

        /**
         * @brief Deserialize the entity list from a file.
         *
         * @param file The file path to deserialize from.
         */
        void Deserialize(const std::string& file);

        template <typename... Components, typename Func>
        void Iterate(Func&& func)
        {
            m_Registry.view<Components...>().each(std::forward<Func>(func));
        }

        template <typename Component, typename Func>
        void Sort(Func&& func)
        {
            m_Registry.sort<Component>(std::forward<Func>(func));
        }

        template <typename Func>
        void IterateEntities(Func&& func)
        {
            m_Registry.each(std::forward<Func>(func));
        }

      private:
        /**
         * @brief EnTT registry actually containing all the entity and component data.
         */
        entt::registry m_Registry;

        /**
         * @brief List of entities created and active.
         */
        std::vector<Entity> ActiveEntityList;

        template <typename Archive, typename... Args>
        /**
         * @brief Private function to serialize the entity list using a specified archive and component types.
         *
         * @tparam Archive The archive type.
         * @tparam Args The component types to serialize.
         * @param file The file path to serialize to.
         */
        void Serialize(const std::string& file) const
        {
            std::ofstream out(file);
            if (out.good())
            {
                Archive json(out);

                json(CEREAL_NVP(ActiveEntityList));

                entt::snapshot snapshot(m_Registry);
                snapshot.entities(json).template component<Args...>(json);

                out << "\n}"; // doesn't close off?

                out.close();
            }
        }

        template <typename Archive, typename... Args>
        /**
         * @brief Private function to deserialize the entity list using a specified archive and component types.
         *
         * @tparam Archive The archive type.
         * @tparam Args The component types to deserialize.
         * @param file The file path to deserialize from.
         */
        void Deserialize(const std::string& file)
        {
            std::ifstream inp(file);

            if (inp.good())
            {
                std::set<uint32_t> entitiesFromSerializedData;

                entt::registry tempRegistry;
                cereal::JSONInputArchive json(inp);

                // Deserialize ActiveEntityList first
                json(ActiveEntityList);

                // Used for deleting entities that are not in the serialized data
                for (const auto& entry : ActiveEntityList)
                {
                    entitiesFromSerializedData.insert(entry.GetHandle());
                }

                // Reconciliation
                m_Registry.each([&](auto entity) {
                    if (entitiesFromSerializedData.find(static_cast<uint32_t>(entity)) ==
                        entitiesFromSerializedData.end())
                    {
                        m_Registry.destroy(entity);
                    }
                });

                entt::snapshot_loader snapLoad(tempRegistry);
                snapLoad.entities(json);

                // Deserialize Components using fold expression
                (snapLoad.component<Args>(json), ...);

                // Copy Components using fold expression
                (tempRegistry.view<Args>().each(
                     [&](auto entity, Args& comp) { m_Registry.emplace_or_replace<Args>(entity, comp); }),
                 ...);

                inp.close();
            }
        }

        friend class Entity; // friend to allow the API for entities to be clean
        friend class Scene;
    };

} // namespace Vakol
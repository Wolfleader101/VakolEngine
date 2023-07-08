#pragma once

#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <entt/entt.hpp>
#include <memory>
#include <vector>

#include "Logger.hpp"

namespace Vakol::Model {

    class Entity;  // pre declared to prevent recursive include
}

using namespace Vakol::Model;

namespace Vakol::Controller {

    class System;
    class Scene;

    /**
     * @class EntityList
     *
     * @brief Wrapper for entt registry and contains a list of all active entities.
     */
    class EntityList {
       public:
        /**
         * @brief Construct a new Entity Manager object.
         */
        EntityList() : m_Registry(), ActiveEntityList(){};

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
        void RemoveEntity(uint32_t Handle);  // can also use entity because type conversion is provided in entity

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

        //-------------- THESE ARE ADDITIONS IN ICT397 ------------------

        /**
         * @brief Initialize the entity list.
         */
        void Init();

        /**
         * @brief Update the entity list.
         *
         * @param d_t The time delta for the update.
         */
        void Update(double d_t);

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
        void privateSerialize(const std::string& file) const {
            std::ofstream out(file);
            if (out.good()) {
                Archive json(out);

                json(CEREAL_NVP(ActiveEntityList));

                entt::snapshot snapshot(m_Registry);
                snapshot.entities(json).component<Args...>(json);

                out << "\n}";  // doesn't close off?

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
        void privateDeserialize(const std::string& file) {
            std::ifstream inp(file);

            if (inp.good()) {
                m_Registry.clear();
                Archive json(inp);
                json(ActiveEntityList);  // fills vector again

                entt::snapshot_loader snapLoad(m_Registry);
                snapLoad.entities(json);
                snapLoad.component<Args...>(json);

                inp.close();
            }
        }

        friend class Entity;  // friend to allow the API for entities to be clean.
        friend class System;
        friend class Scene;
    };

}  // namespace Vakol::Controller
#pragma once

#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <entt/entt.hpp>
#include <memory>
#include <vector>

#include "Logger.hpp"
#include "Controller/LuaState.hpp"

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
     *
     * @brief Wrapper for entt registry and contains a list of all active entities
     *
     */
    class EntityList {
       public:
        /**
         * @brief Construct a new Entity Manager object
         *
         */
        EntityList() : m_Registry(), ActiveEntityList(){};

        /**
         * @brief Create an Entity object
         *
         * @return Entity created
         */
        Entity CreateEntity();

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
        void RemoveEntity(uint32_t Handle);  // can also use entity because type conversion provided in entity

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
        std::vector<Entity>& GetEntityVec();

        //-------------- THESE ARE ADDITIONS IN ICT397 ------------------

        void Init();

        void Update(double d_t);

        void Serialize(const std::string& file) const;
        void Deserialize(const std::string& file);

       private:
        /**
         * @brief EnTT registry actually containing all the entity and comp data
         */
        entt::registry m_Registry;

        /**
         * @brief list of entities created and active
         */
        std::vector<Entity> ActiveEntityList;

        template <typename Archive, typename... Args>
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
        void privateDeserialize(const std::string& file) {
            std::ifstream inp(file);

            if (inp.good()) 
            {
                m_Registry.clear();
                Archive json(inp);
                json(ActiveEntityList);  // fills vector again

                
                entt::snapshot_loader snapLoad(m_Registry);
                snapLoad.entities(json);
                snapLoad.component<Args...>(json);

                inp.close();
            }
        }

        // template <typename Archive, typename... Args>
        // void privateDeserialize(const std::string& file, std::shared_ptr<LuaState> lua, Scene* scene) {
        //     std::ifstream inp(file);

        //     if (inp.good()) {
                

        //         m_Registry.clear();
        //         ActiveEntityList.clear();

        //         Archive json(inp);
        //         json(ActiveEntityList);  // fills vector again
                
                
        //         entt::snapshot_loader snapLoad(m_Registry);
        //         snapLoad.entities(json);
        //         snapLoad.component<Args...>(json);



        //         System::Script_Init(lua, *this, scene);

        //         //inp.close();


        //         inp.seekg(0, inp.beg); //reset file pointer to start of file

        //         entt::registry tempReg;
        //         entt::snapshot_loader tempSnap(tempReg);
        //         tempSnap.entities(json);
        //         tempSnap.component<Args...>(json);

        //         //do n^2 view here
                
        //         m_Registry.group<Components::Script, Components::Transform, Components::GUID, Components::Tag>().each(
        //             [&](auto& script, auto& trans, auto& guid, auto& tag)
        //             {
        //                     tempReg.group<Components::Script, Components::Transform, Components::GUID, Components::Tag>().each(
        //                     [&](auto& scriptTemp, auto& transTemp, auto& guidTemp, auto& tagTemp)
        //                     {
        //                         if(guid == guidTemp)
        //                         {
        //                             Controller::ConvertMapToSol(scriptTemp.data, script.state); //convert map to sol state (lua
        //                             script.state = scriptTemp.state;

        //                             trans = transTemp;

        //                             tag = tagTemp;
        //                         }
        //                     });
        //             });

                
        //         inp.close();
        //     }

        //     
        // }

        friend class Entity;  // friend to allow the api for entities to be clean.
        friend class System;
        friend class Scene;
    };

}  // namespace Vakol::Controller
#include <fstream>
#include <cereal/archives/json.hpp>

#include <Controller/EntityList.hpp>
#include <Controller/Scene.hpp>

#include "Serializable.hpp"



namespace Vakol::Controller
{
    // void EntityList::Serialize(const std::string& file) const 
    // {
    //     std::ofstream output(file);

    //     if(output.good())
    //     {
    //         cereal::JSONOutputArchive json(output);
    //         auto& x = entt::basic_snapshot<entt::registry>(m_Registry);

    //         json(x);
    //     }

    //     output.close();   
    // }

    // void EntityList::Deserialize(const std::string& file) 
    // {

    // }

    // void Scene::Serialize(const std::string& folder) const
    // {
    //     Serializable json(entityList);

    //     json.Serialize("/" + folder + "/" + name + "/EntityList.json");
        
    //     //json.Serialize camera...
        
    // }

    // void Scene::Deserialize(const std::string& folder)
    // {
            
    // }
}
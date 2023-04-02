#pragma once

#include <vector>
#include <string>


#include "Mesh.hpp"

namespace Vakol::Model::Assets {

    struct Model : public Asset {
       private:
        Model(const std::string& file);  // this constructor would load the data with assimp

       public:
        std::vector<Mesh> meshes;
    };

}  // namespace Vakol::Model::Assets
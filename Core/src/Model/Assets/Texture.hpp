#pragma once

#include <string>

namespace Vakol::Model::Assets 
{
    struct Texture 
    {
        std::string path;
        std::string type;
        unsigned int id;

       private:
        int width;
        int height;
        int channels;
    };
}  // namespace Vakol::Model::Assets
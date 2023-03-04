#pragma once

#include <memory>

namespace Vakol::Model {

struct Texture {
    Texture() = default;

    int width = 0;
    int height = 0;
    int colorDepth = 0;

   private:
    std::unique_ptr<unsigned char*> data;
    unsigned int id;
};
}  // namespace Vakol::Model
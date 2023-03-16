#pragma once

#include <string>

namespace Vakol::Model {

    struct GameConfig {
        std::string name;
        int windowWidth;
        int windowHeight;
        std::string rendererType;
    };
}  // namespace Vakol::Model
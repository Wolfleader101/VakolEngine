#pragma once

#include <string>

namespace Vakol
{

    /**
     * @brief Struct representing a Game Configuration.
     */
    struct GameConfig
    {
        std::string name;         // Name of the game.
        int windowWidth;          // Width of the game window.
        int windowHeight;         // Height of the game window.
        std::string rendererType; // Type of renderer used in the game.
    };
} // namespace Vakol

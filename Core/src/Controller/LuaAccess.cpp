#include "LuaAccess.hpp"

#include <Model/GameConfig.hpp>

using namespace Vakol::Model;
namespace Vakol::Controller {
    void RegisterGameConfig(sol::state& lua) {
        // auto gameConfigType = lua.new_usertype<GameConfig>("game_config");

        // gameConfigType["name"] = &GameConfig::name;
        // gameConfigType["window"]["w"] = &GameConfig::windowWidth;
        // gameConfigType["window"]["h"] = &GameConfig::windowHeight;
        // gameConfigType["renderer"] = &GameConfig::rendererType;
    }
}  // namespace Vakol::Controller
#include "LuaAccess.hpp"

#include "Rendering/Assets/Mesh.hpp"
#include "Rendering/Assets/Model.hpp"
#include "Rendering/Assets/Texture.hpp"

#include "AssetLoader/AssetLoader.hpp"

namespace Vakol
{
    void RegisterAssetLoader(sol::state& lua)
    {
        lua.set_function("load_texture", [](const std::string& path, const bool flip) {
            if (const auto& texture = std::make_shared<Rendering::Assets::Texture>(AssetLoader::GetTexture(path, 0));
                texture == nullptr)
            {
                return false;
            }

            return true;
        });

        lua.set_function("load_model", [](const std::string& path, const float scale = 1.0f) {
            if (const auto& model = std::make_shared<Rendering::Assets::Model>(AssetLoader::GetModel(path, scale));
                model == nullptr)
            {
                return false;
            }

            return true;
        });
    }

    void RegisterModel(sol::state& lua)
    {
    }

    void RegisterMesh(sol::state& lua)
    {
    }

    void RegisterMaterial(sol::state& lua)
    {
    }

    void RegisterShader(sol::state& lua)
    {
    }

} // namespace Vakol
#include "LuaAccess.hpp"

#include "Rendering/Assets/Mesh.hpp"
#include "Rendering/Assets/Model.hpp"
#include "Rendering/Assets/Texture.hpp"

#include "AssetLoader/AssetLoader.hpp"
#include "Logger/Logger.hpp"

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
        auto model_type = lua.new_usertype<Rendering::Assets::Model>("Model");
    }

    void RegisterMesh(sol::state& lua)
    {
    }

    void RegisterMaterial(sol::state& lua)
    {
    }

    void RegisterShader(sol::state& lua)
    {
        lua.set_function("set_bool", [](const Entity* ent, const char* name, const bool value) {
            if (!ent->HasComponent<Rendering::Drawable>())
            {
                VK_ERROR("No Drawable component found on entity!");

                return;
            }

            AssetLoader::SetBool(AssetLoader::GetShader(ent->GetComponent<Rendering::Drawable>().shaderID), name,
                                 value);
        });

        lua.set_function("set_int", [](const Entity* ent, const char* name, const int value) {
            if (!ent->HasComponent<Rendering::Drawable>())
            {
                VK_ERROR("No Drawable component found on entity!");

                return;
            }

            AssetLoader::SetInt(AssetLoader::GetShader(ent->GetComponent<Rendering::Drawable>().shaderID), name, value);
        });

        lua.set_function("set_float", [](const Entity* ent, const char* name, const float value) {
            if (!ent->HasComponent<Rendering::Drawable>())
            {
                VK_ERROR("No Drawable component found on entity!");

                return;
            }

            AssetLoader::SetFloat(AssetLoader::GetShader(ent->GetComponent<Rendering::Drawable>().shaderID), name,
                                  value);
        });

        lua.set_function("set_vec2v", [](const Entity* ent, const char* name, const Math::Vec2& value) {
            if (!ent->HasComponent<Rendering::Drawable>())
            {
                VK_ERROR("No Drawable component found on entity!");

                return;
            }

            AssetLoader::SetVec2(AssetLoader::GetShader(ent->GetComponent<Rendering::Drawable>().shaderID), name,
                                 value);
        });

        lua.set_function("set_vec3v", [](const Entity* ent, const char* name, const Math::Vec3& value) {
            if (!ent->HasComponent<Rendering::Drawable>())
            {
                VK_ERROR("No Drawable component found on entity!");

                return;
            }

            AssetLoader::SetVec3(AssetLoader::GetShader(ent->GetComponent<Rendering::Drawable>().shaderID), name,
                                 value);
        });

        lua.set_function("set_vec4v", [](const Entity* ent, const char* name, const Math::Vec4& value) {
            if (!ent->HasComponent<Rendering::Drawable>())
            {
                VK_ERROR("No Drawable component found on entity!");

                return;
            }

            AssetLoader::SetVec4(AssetLoader::GetShader(ent->GetComponent<Rendering::Drawable>().shaderID), name,
                                 value);
        });

        lua.set_function(
            "set_mat3", [](const char* name, const bool transpose, const Math::Mat3& value) {
                if (!ent->HasComponent<Rendering::Drawable>())
                {
                    VK_ERROR("No Drawable component found on entity!");

                    return;
                }

                AssetLoader::SetMat3(AssetLoader::GetShader(ent->GetComponent<Rendering::Drawable>().shaderID), name,
                                     transpose, value);
            });

        lua.set_function(
            "set_mat4", [](const Entity* ent, const char* name, const bool transpose, const Math::Mat4& value) {
                if (!ent->HasComponent<Rendering::Drawable>())
                {
                    VK_ERROR("No Drawable component found on entity!");

                    return;
                }

                AssetLoader::SetMat4(AssetLoader::GetShader(ent->GetComponent<Rendering::Drawable>().shaderID), name,
                                     transpose, value);
            });
    }

} // namespace Vakol
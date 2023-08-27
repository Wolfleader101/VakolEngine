#include "LuaAccess.hpp"

#include "Rendering/Assets/Mesh.hpp"
#include "Rendering/Assets/Model.hpp"
#include "Rendering/Assets/Texture.hpp"

#include "AssetLoader/AssetLoader.hpp"
#include "Logger/Logger.hpp"
#include "Rendering/RenderAPI.hpp"

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

        model_type.set_function("get_shader", [](const Rendering::Assets::Model* model) {
            return Rendering::RenderAPI::GetShader(model->meshes.at(0).material->shaderID);
        });
    }

    void RegisterMesh(sol::state& lua)
    {
    }

    void RegisterMaterial(sol::state& lua)
    {
    }

    void RegisterShader(sol::state& lua)
    {
        lua.set_function("set_bool", [](const unsigned int shader, const char* name, const bool value) {
            Rendering::RenderAPI::SetBool(shader, name, value);
        });

        lua.set_function("set_int", [](const unsigned int shader, const char* name, const int value) {
            Rendering::RenderAPI::SetInt(shader, name, value);
        });

        lua.set_function("set_float", [](const unsigned int shader, const char* name, const float value) {
            Rendering::RenderAPI::SetFloat(shader, name, value);
        });

        lua.set_function("set_vec2v", [](const unsigned int shader, const char* name, const Math::Vec2& value) {
            Rendering::RenderAPI::SetVec2(shader, name, value);
        });

        lua.set_function("set_vec3v", [](const unsigned int shader, const char* name, const Math::Vec3& value) {
            Rendering::RenderAPI::SetVec3(shader, name, value);
        });

        lua.set_function("set_vec4v", [](const unsigned int shader, const char* name, const Math::Vec4& value) {
            Rendering::RenderAPI::SetVec4(shader, name, value);
        });

        lua.set_function(
            "set_mat3", [](const unsigned int shader, const char* name, const bool transpose, const Math::Mat3& value) {
                Rendering::RenderAPI::SetMat3(shader, name, transpose, value);
            });

        lua.set_function(
            "set_mat4", [](const unsigned int shader, const char* name, const bool transpose, const Math::Mat4& value) {
                Rendering::RenderAPI::SetMat4(shader, name, transpose, value);
            });
    }

} // namespace Vakol
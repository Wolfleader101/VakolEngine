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

        // lua.set_function("load_model", [](const std::string& path, const float scale = 1.0f) {
        //     if (const auto& model = std::make_shared<Rendering::Assets::Model>(AssetLoader::GetModel(path, scale));
        //         model == nullptr)
        //     {
        //         return false;
        //     }

        //    return true;
        //});
    }

    void RegisterModel(sol::state& lua)
    {
        auto model_type = lua.new_usertype<Rendering::Assets::Model>("Model");

        model_type.set_function("get_mesh_count", [](const Rendering::Assets::Model& model) {
            return static_cast<int>(model.meshes.size());
        });

        model_type.set_function("get_mesh",
                                [](Rendering::Assets::Model& model, const int index) -> Rendering::Assets::Mesh& {
                                    if (model.meshes.size() > index)
                                        return model.meshes.at(index);

                                    VK_ERROR("Unable to fetch mesh at index: {0}", index);

                                    return model.meshes.at(0);
                                });

        model_type.set_function("get_half_extents",
                                [](const Rendering::Assets::Model& model, const int index) -> Math::Vec3 {
                                    if (model.meshes.size() > index)
                                        return model.meshes.at(index).bounds.halfExtents;

                                    VK_ERROR("Unable to fetch half extents at index: {0}", index);

                                    return model.meshes.at(0).bounds.halfExtents;
                                });
    }

    void RegisterMesh(sol::state& lua)
    {
        auto mesh_type = lua.new_usertype<Rendering::Assets::Mesh>("Mesh");

        mesh_type.set("material", &Rendering::Assets::Mesh::material);
    }

    void RegisterMaterial(sol::state& lua)
    {
        auto material_type = lua.new_usertype<Rendering::Assets::Material>("Material");

        material_type.set_function("get_shader", [](Rendering::Assets::Material& material) {
            return Rendering::RenderAPI::GetShader(material.shaderID);
        });

        material_type.set_function("set_ambient_color",
                                   [](Rendering::Assets::Material& material, const Math::Vec4& ambient) {
                                       material.properties.ambient_color = ambient;
                                   });

        material_type.set_function("set_diffuse_color",
                                   [](Rendering::Assets::Material& material, const Math::Vec4& diffuse) {
                                       material.properties.diffuse_color = diffuse;
                                   });

        material_type.set_function("set_specular_color",
                                   [](Rendering::Assets::Material& material, const Math::Vec4& specular) {
                                       material.properties.specular_color = specular;
                                   });

        material_type.set_function("set_emissive_color",
                                   [](Rendering::Assets::Material& material, const Math::Vec4& emissive) {
                                       material.properties.emissive_color = emissive;
                                   });

        material_type.set_function("set_light_direction",
                                   [](Rendering::Assets::Material& material, const Math::Vec3& direction) {
                                       material.properties.light.LIGHT_DIRECTION = Math::Vec4(direction, 0.0);
                                   });

        material_type.set_function("set_light_position",
                                   [](Rendering::Assets::Material& material, const Math::Vec3& position) {
                                       material.properties.light.LIGHT_DIRECTION = Math::Vec4(position, 1.0f);
                                   });

        material_type.set_function("set_uv_offset",
                                   [](Rendering::Assets::Material& material, const Math::Vec2& offset) {
                                       material.properties.uv_offset = offset;
                                   });

        material_type.set_function("set_opacity",
                                   [](Rendering::Assets::Material& material, const float opacity) {
                                       material.properties.opacity = opacity;
                                   });

        material_type.set_function("use_lighting", [](Rendering::Assets::Material& material, const bool enabled) {
            material.properties.use_lighting = enabled;
        });

        material_type.set_function("use_textures", [](Rendering::Assets::Material& material, const bool enabled) {
            material.properties.use_textures = enabled;
        });
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
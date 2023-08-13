#include "LuaAccess.hpp"

#include "AssetLoader/AssetLoader.hpp"
#include "Rendering/Assets/Mesh.hpp"
#include "Rendering/Assets/Model.hpp"
#include "Rendering/Instance.hpp"

namespace Vakol
{

    using namespace Vakol::Assets;

    void RegisterAssetLoader(sol::state& lua)
    {
        lua.set_function("load_texture", [](const std::string& path, const bool gamma, const bool flip) {
            if (const auto texture = AssetLoader::GetTexture(path, gamma, flip); texture == nullptr)
                return false;

            return true;
        });

        lua.set_function("load_model", [](const std::string& path, const float scale = 1.0f,
                                          const bool animated = false, const bool backfaceCull = true) {
            if (const auto& [model, animator] = AssetLoader::GetModel(path, scale, animated, backfaceCull);
                model == nullptr)
                return false;

            return true;
        });

        lua.set_function("load_shader", [](const std::string& path) {
            if (const auto& shader = AssetLoader::GetShader(path); shader == nullptr)
                return false;

            return true;
        });
    }

    void RegisterModel(sol::state& lua)
    {
        auto model_type = lua.new_usertype<Assets::Model>("Model");
        model_type.set_function("get_mesh_count", &Assets::Model::nMeshes);
        model_type.set_function("get_mesh", &Assets::Model::mesh);

        model_type.set_function("get_shader", &Assets::Model::shader);

        lua.set_function("instantiate_model",
                         [](const std::shared_ptr<Assets::Model>& model, const std::vector<Math::Mat4>& matrices,
                            const int amount) { CreateInstances(model->meshes(), matrices, amount, 3); });
    }

    void RegisterMesh(sol::state& lua)
    {
        auto mesh_type = lua.new_usertype<Mesh>("Mesh");

        mesh_type.set_function("get_material", &Mesh::GetMaterial);
    }

    void RegisterMaterial(sol::state& lua)
    {
        auto material_type = lua.new_usertype<Assets::Material>("Material");
        material_type.set_function("get_texture", &Assets::Material::GetTexture);

        material_type.set_function("get_ambient_color", &Assets::Material::GetAmbientColor);
        material_type.set_function("get_diffuse_color", &Assets::Material::GetDiffuseColor);
    }

    void RegisterShader(sol::state& lua)
    {
        auto shader_type = lua.new_usertype<Shader>("Shader");
        shader_type.set_function("set_bool", [](const Shader* shader, const std::string& name, const bool value) {
            shader->SetBool(name.c_str(), value);
        });

        shader_type.set_function("set_int", &Shader::SetInt);

        shader_type.set_function("set_float", &Shader::SetFloat);

        shader_type.set_function("set_vec2v",
                                 sol::resolve<void(const char*, const Math::Vec2&) const>(&Shader::SetVec2));
        shader_type.set_function("set_vec3v",
                                 sol::resolve<void(const char*, const Math::Vec3&) const>(&Shader::SetVec3));
        shader_type.set_function("set_vec4v",
                                 sol::resolve<void(const char*, const Math::Vec4&) const>(&Shader::SetVec4));

        shader_type.set_function("set_vec2", sol::resolve<void(const char*, float, float) const>(&Shader::SetVec2));
        shader_type.set_function("set_vec3",
                                 sol::resolve<void(const char*, float, float, float) const>(&Shader::SetVec3));
        shader_type.set_function("set_vec4",
                                 sol::resolve<void(const char*, float, float, float, float) const>(&Shader::SetVec4));
    }

} // namespace Vakol
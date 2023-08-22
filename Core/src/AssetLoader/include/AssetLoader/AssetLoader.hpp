#pragma once

#include <memory>
#include <unordered_map>

#include "Rendering/Assets/Model.hpp"
#include "Rendering/Assets/Texture.hpp"

#include "ModelLibrary.hpp"
#include "ShaderLibrary.hpp"
#include "TextureLibrary.hpp"

namespace Vakol
{
    namespace Rendering::Assets
    {
        struct Shader;
    }

    /**
     * @brief AssetLoader class for loading game assets.
     */
    class AssetLoader
    {
      public:
        /**
         * @brief Path to the model files.
         */
        static std::string model_path;

        /**
         * @brief Path to the texture files.
         */
        static std::string texture_path;

        /**
         * @brief Path to the shader files.
         */
        static std::string shader_path;

        static void AddShader(const std::string& shaderID, unsigned int shader);

        static void SetBool(unsigned int shader, const char* name, bool value);
        static void SetInt(unsigned int shader, const char* name, int value);
        static void SetFloat(unsigned int shader, const char* name, float value);

        static void SetVec2(unsigned int shader, const char* name, const Math::Vec2& value);
        static void SetVec3(unsigned int shader, const char* name, const Math::Vec3& value);
        static void SetVec4(unsigned int shader, const char* name, const Math::Vec4& value);

        static void SetMat3(unsigned int shader, const char* name, bool transpose, const Math::Mat3& value);
        static void SetMat4(unsigned int shader, const char* name, bool transpose, const Math::Mat4& value);

        static Rendering::Assets::Model& FindModel(const std::string& path);

        static Rendering::Assets::Model& GetModel(const std::string& path, float scale = 1.0f);
        static const std::vector<Rendering::Assets::Mesh>& GetMeshes(const std::string& modelID);

        static unsigned int GetShader(const std::string& shaderID);

        static Rendering::Assets::Texture& GetTexture(const std::string& path, unsigned int type, int levels = 1);
        static Rendering::Assets::Texture& GetTexture(const std::string& path, unsigned int type, int size,
                                                      const void* data, int levels = 1);
        static void ReplaceTexture(const std::string& modelID, const std::string& srcPath, unsigned int srcType,
                                   const std::string& dstPath, unsigned int dstType);

      private:
        static ModelLibrary m_modelLibrary;
        static ShaderLibrary m_shaderLibrary;
        static TextureLibrary m_textureLibrary;
    };

} // namespace Vakol

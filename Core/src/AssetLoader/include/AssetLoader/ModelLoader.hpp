#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "AssetLoader/FileLoader.hpp"
#include "AssetLoader/AssetLoader.hpp"

#include "Rendering/RenderCommand.hpp"
#include "Rendering/Assets/Model.hpp"
#include "Rendering/Assets/Animation.hpp"
#include "Rendering/Assets/Material.hpp"
#include "Rendering/Assets/Texture.hpp"
#include "Rendering/RenderData.hpp"

#include "Logger/Logger.hpp"

#include "Math/Math.hpp"

namespace Vakol
{
    /**
     * \brief
     * \param path the path of the model
     * \param scale the global scale of the model
     * \param success does the model successfully load?
     * \return
     */
    bool ImportModel(Rendering::Assets::Model& model, const char* path, const float scale);
} // namespace Vakol

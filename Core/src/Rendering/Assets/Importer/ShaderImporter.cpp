#include "ShaderImporter.hpp"

#include "Controller/Logger.hpp"
#include "Controller/AssetLoader/FileLoader.hpp"

#include <vector>

namespace Vakol::Rendering::Assets::Importer
{
    bool GetShaderSources(std::vector<std::string>&& paths, Shader& shader);

    Shader ImportShader(const std::string& path, bool& success)
    {
        Shader shader;

        std::vector<std::string> paths;

        const auto directory = path.substr(0, path.find_last_of('.'));

        const char* extensions[] = {".vert", ".geom", ".tesc", ".tese", ".frag"};

        for (const auto& extension : extensions)
        {
            if (const auto shaderPath = directory + extension; FileExists(shaderPath))
                paths.emplace_back(shaderPath);
            else
                paths.emplace_back("");
        }

        shader.path = path;
        success = GetShaderSources(std::move(paths), shader);

        return shader;
    }

    bool GetShaderSources(std::vector<std::string>&& paths, Shader& shader)
    {
        std::vector<std::string> sources;

        if (paths[0].empty() || paths[4].empty())
        {
            VK_ERROR("VERTEX AND/OR FRAGMENT SHADER WAS NOT FOUND!");

            return false;
        }

        sources.emplace_back(LoadFile(paths[0]));

        if (paths[1].empty())
        {
            sources.emplace_back("");
        }
        else
            sources.emplace_back(LoadFile(paths[1]));

        if (paths[2].empty())
        {
            sources.emplace_back("");
        }
        else
            sources.emplace_back(LoadFile(paths[2]));

        if (paths[3].empty())
        {
            sources.emplace_back("");
        }
        else
            sources.emplace_back(LoadFile(paths[3]));

        sources.emplace_back(LoadFile(paths[4]));

        shader.sources = std::move(sources);

        return true;
    }
}
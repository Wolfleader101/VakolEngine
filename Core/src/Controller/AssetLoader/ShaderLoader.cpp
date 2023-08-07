#include "ShaderLoader.hpp"

#include <glad/glad.h>

#include <Controller/AssetLoader/FileLoader.hpp>
#include <Controller/Logger.hpp>
#include <iostream>
#include <vector>

unsigned int LoadShader(const std::string&, const std::string&, const std::string&, const std::string&,
                        const std::string&);

unsigned int CompileGLShader(std::string&&, std::string&&, std::string&&, std::string&&, std::string&&);
int CheckCompileErrors(const unsigned int, const std::string&);

namespace Vakol::Controller {
    unsigned int LoadShader(const std::string& path) {
        const std::string directory = path.substr(0, path.find_last_of('.'));

        const std::string extensions[] = {".vert", ".frag", ".geom", ".tesc", ".tese"};

        std::vector<std::string> shaders;

        for (const auto& extension : extensions) {
            if (std::string file = directory + extension; FileExists(file))
                shaders.push_back(file);
            else
                shaders.emplace_back("null");
        }

        return ::LoadShader(shaders[0], shaders[1], shaders[2], shaders[3], shaders[4]);
    }
}  // namespace Vakol::Controller

unsigned int LoadShader(const std::string& vPath, const std::string& fPath, const std::string& gPath,
                        const std::string& tcPath, const std::string& tePath) {
    // Vertex, Fragment Code
    std::string v_code;
    std::string f_code;
    std::string g_code;
    std::string tc_code;
    std::string te_code;

    if (vPath != "null")
        v_code = LoadFile(vPath);
    else
        VK_ERROR("Unable to load vertex shader at path: {0}. Double-check the out folder", vPath);

    if (fPath != "null")
        f_code = LoadFile(fPath);
    else
        VK_ERROR("Unable to load fragment shader at path {0}. Double-check the out folder", fPath);

    if (gPath != "null") g_code = LoadFile(std::string(gPath));
    if (tcPath != "null") tc_code = LoadFile(std::string(tcPath));
    if (tePath != "null") te_code = LoadFile(std::string(tePath));

    return CompileGLShader(std::move(v_code), std::move(f_code), std::move(g_code), std::move(tc_code),
                           std::move(te_code));
}

unsigned int CompileGLShader(std::string&& v_code, std::string&& f_code, std::string&& g_code, std::string&& tc_code,
                             std::string&& te_code) {
    // Vertex Shader Code (same thing as v_code)
    const auto vs_code = v_code.c_str();
    // Fragment Shader Code (same thing as f_code)
    const auto fs_code = f_code.c_str();

    auto success = 0;

    const unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vs_code, nullptr);
    glCompileShader(vertex);
    success = CheckCompileErrors(vertex, "VERTEX");

    const unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fs_code, nullptr);
    glCompileShader(fragment);
    success = CheckCompileErrors(fragment, "FRAGMENT");

    unsigned int geometry = 0;

    if (!g_code.empty()) {
        const auto gs_code = g_code.c_str();

        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gs_code, nullptr);
        glCompileShader(geometry);
        success = CheckCompileErrors(geometry, "GEOMETRY");
    }

    unsigned int tess_control = 0;

    if (!tc_code.empty()) {
        const auto tcs_code = tc_code.c_str();

        tess_control = glCreateShader(GL_TESS_CONTROL_SHADER);
        glShaderSource(tess_control, 1, &tcs_code, nullptr);
        glCompileShader(tess_control);
        success = CheckCompileErrors(tess_control, "TESS_CONTROL");
    }

    unsigned int tess_eval = 0;

    if (!te_code.empty()) {
        const auto tes_code = te_code.c_str();

        tess_eval = glCreateShader(GL_TESS_EVALUATION_SHADER);
        glShaderSource(tess_eval, 1, &tes_code, nullptr);
        glCompileShader(tess_eval);
        success = CheckCompileErrors(tess_eval, "TESS_EVALUATION");
    }

    if (success)
        VK_INFO("Shader compiled with no known issues");
    else
        VK_WARN("Compile errors were detected, better go and re-check your code.");

    const auto id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);

    if (!g_code.empty()) glAttachShader(id, geometry);

    if (!tc_code.empty()) glAttachShader(id, tess_control);

    if (!te_code.empty()) glAttachShader(id, tess_eval);

    glLinkProgram(id);
    success = CheckCompileErrors(id, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    if (!g_code.empty()) glDeleteShader(geometry);

    if (!tc_code.empty()) glDeleteShader(tess_control);

    if (!te_code.empty()) glDeleteShader(tess_eval);

    std::cout << std::endl;

    return id;
}

int CheckCompileErrors(const unsigned int id, const std::string& type) {
    int success;
    char info_log[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(id, 1024, nullptr, info_log);
            VK_ERROR("ERROR::SHADER_COMPILATION:: TYPE = {0}\n{1}", type, info_log);
        }

        return success;
    } else {
        glGetProgramiv(id, GL_LINK_STATUS, &success);

        if (!success) {
            glGetProgramInfoLog(id, 1024, nullptr, info_log);
            VK_ERROR("ERROR::PROGRAM_LINKING:: TYPE = {0}\n{1}", type, info_log);
        }

        return success;
    }
}
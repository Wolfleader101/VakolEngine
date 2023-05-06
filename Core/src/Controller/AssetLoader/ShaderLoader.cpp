#include "ShaderLoader.hpp"

#include <glad/glad.h>

#include <vector>
#include <iostream>

#include <Controller/Logger.hpp>

#include <Controller/AssetLoader/FileLoader.hpp>

const unsigned int LoadShader(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);

const unsigned int CompileGLShader(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
const int CheckCompileErrors(const unsigned int, const std::string&);

namespace Vakol::Controller
{
    const unsigned int LoadShader(const std::string& path)
    {
        std::string directory, file;

        directory = path.substr(0, path.find_last_of('.'));

        std::string extensions[] = {".vert", ".frag", ".geom", ".tesc", ".tese", ".comp"};

        std::vector<std::string> shaders;

        for (int i = 0; i < 6; ++i)
        {
            file = directory + extensions[i];

            if (FileExists(file))
                shaders.push_back(file);
            else
                shaders.push_back("null");
        }

        return ::LoadShader(shaders[0], shaders[1], shaders[2], shaders[3], shaders[4], shaders[5]);
    }
}

const unsigned int LoadShader(const std::string& vPath, const std::string& fPath, const std::string& gPath, const std::string& tcPath, const std::string& tePath, const std::string& csPath)
{
    // Vertex, Fragment Code
    std::string v_code;
    std::string f_code;
    std::string g_code;
    std::string tc_code;
    std::string te_code;
    std::string cmp_code;

    if (vPath != "null")
        v_code = LoadFile(vPath);
    else
        VK_ERROR("Unable to load vertex shader at path: {0}. Double-check the out folder", vPath);
    
    if (fPath != "null")
        f_code = LoadFile(fPath);
    else
        VK_ERROR("Unable to load fragment shader at path {0}. Double-check the out folder", fPath);
    
    if (gPath != "null")
        g_code = LoadFile(std::string(gPath));
    if (tcPath != "null")
        tc_code = LoadFile(std::string(tcPath));
    if (tePath != "null")
        te_code = LoadFile(std::string(tePath));
    if (csPath != "null")
        cmp_code = LoadFile(std::string(csPath));
        
    return CompileGLShader(v_code, f_code, g_code, tc_code, te_code, cmp_code);
}

const unsigned int CompileGLShader(const std::string& v_code, const std::string& f_code, const std::string& g_code, const std::string& tc_code, const std::string& te_code, const std::string& cs_code)
{
    // Vertex Shader Code (same thing as v_code)
    const auto vs_code = v_code.c_str();
    // Fragment Shader Code (same thing as f_code)
    const auto fs_code = f_code.c_str();

    auto success = 0;

    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vs_code, NULL);
	glCompileShader(vertex);
    success = CheckCompileErrors(vertex, "VERTEX");

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fs_code, NULL);
	glCompileShader(fragment);
    success = CheckCompileErrors(fragment, "FRAGMENT");

    unsigned int geometry = 0;

    if (!g_code.empty())
    {
        const auto gs_code = g_code.c_str();

        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gs_code, NULL);
        glCompileShader(geometry);
        success = CheckCompileErrors(geometry, "GEOMETRY");
    }

    unsigned int tessControl = 0;

    if (!tc_code.empty())
    {
        const auto tcs_code = tc_code.c_str();

        tessControl = glCreateShader(GL_TESS_CONTROL_SHADER);
        glShaderSource(tessControl, 1, &tcs_code, NULL);
        glCompileShader(tessControl);
        success = CheckCompileErrors(tessControl, "TESS_CONTROL");
    }

    unsigned int tessEval = 0;

    if (!te_code.empty())
    {
        const auto tes_code = te_code.c_str();

        tessEval = glCreateShader(GL_TESS_EVALUATION_SHADER);
        glShaderSource(tessEval, 1, &tes_code, NULL);
        glCompileShader(tessEval);
        success = CheckCompileErrors(tessEval, "TESS_EVALUATION");
    }

    unsigned int compShader = 0;

    if (!cs_code.empty())
    {
        const auto comp_code = cs_code.c_str();

        compShader = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(compShader, 1, &comp_code, NULL);
        success = CheckCompileErrors(compShader, "COMPUTE");
    }

	auto id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);

    if (!g_code.empty())
        glAttachShader(id, geometry);

    if (!tc_code.empty())
        glAttachShader(id, tessControl);
    
    if (!te_code.empty())
        glAttachShader(id, tessEval);

    if (!cs_code.empty())
        glAttachShader(id, compShader);

	glLinkProgram(id);
    success = CheckCompileErrors(id, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);

    if (!g_code.empty())
        glDeleteShader(geometry);
    
    if (!tc_code.empty())
        glDeleteShader(tessControl);
    
    if (!te_code.empty())
        glDeleteShader(tessEval);

    if (!cs_code.empty())
        glDeleteShader(compShader);

    std::cout << std::endl;
    
    if (success) VK_INFO("Shader compiled with no known issues");
    else VK_WARN("Compile errors were detected, better go and re-check your code.");

    return id;
}

const int CheckCompileErrors(const unsigned int id, const std::string& type)
{
    int success;
    char infoLog[1024];

    if (type != "PROGRAM")
    {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(id, 1024, NULL, infoLog);
            VK_ERROR("ERROR::SHADER_COMPILATION:: TYPE = {0}\n{1}", type, infoLog);
        }
        
        return success;
    }
    else
    {
        glGetProgramiv(id, GL_LINK_STATUS, &success);

        if (!success)
        {
            glGetProgramInfoLog(id, 1024, NULL, infoLog);
            VK_ERROR("ERROR::PROGRAM_LINKING:: TYPE = {0}\n{1}", type, infoLog);
        }

        return success;
    }
}
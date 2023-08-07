#include "ScriptEngine.hpp"

#include "Controller/Logger.hpp"
#include "LuaAccess.hpp"

static std::string getStatusString(const sol::call_status status)
{
    switch (status)
    {
    case sol::call_status::ok:
        return "ok";
    case sol::call_status::yielded:
        return "yielded";
    case sol::call_status::runtime:
        return "runtime";
    case sol::call_status::memory:
        return "memory";
    case sol::call_status::handler:
        return "handler";
    case sol::call_status::gc:
        return "gc";
    case sol::call_status::syntax:
        return "syntax";
    case sol::call_status::file:
        return "file";
    default:
        return "unknown";
    }
}

namespace Vakol
{

    ScriptEngine::ScriptEngine() : m_state()
    {
        m_state.open_libraries(sol::lib::base);
        m_state.open_libraries(sol::lib::math);
        m_state.open_libraries(sol::lib::string);

        this->RegisterTypes();
        this->RegisterFunctions();
    }
    // ScriptEngine::~ScriptEngine() { m_scripts.clear(); }

    void ScriptEngine::InitScript(LuaScript& script)
    {
        RunFunction(script.env, "init", true);
    }

    void ScriptEngine::UpdateScript(LuaScript& script)
    {
        RunFunction(script.env, "update", true);
    }

    void ScriptEngine::TickScript(LuaScript& script)
    {
        RunFunction(script.env, "tick", true);
    }

    LuaScript ScriptEngine::CreateScript(const std::string& scriptPath)
    {
        LuaScript script;

        script.path = scriptPath;

        script.env = sol::environment(m_state, sol::create, m_state.globals());

        this->SetScriptVariable(script, "state", script.env);

        //! run the script on creation (load up global vars etc)
        this->RunFile(script.env, scriptPath);

        return script;
    }

    LuaTable ScriptEngine::CreateTable()
    {
        return m_state.create_table();
    }

    LuaType ScriptEngine::GetGlobalVariable(const std::string& name)
    {
        return GetVariable(m_state.globals(), name);
    }

    LuaType ScriptEngine::GetScriptVariable(const LuaScript& script, const std::string& varName)
    {
        return GetVariable(script.env, varName);
    }

    LuaType ScriptEngine::GetVariable(sol::environment env, const std::string& varName)
    {
        sol::object var = env[varName];

        if (!var.valid())
        {
            std::ostringstream oss;
            oss << "Lua script execution error: Variable '" << varName << "' not found";
            std::string errorMsg = oss.str();

            VK_ERROR(errorMsg);
            return LuaType();
        }

        return var;
    }

    void ScriptEngine::RunFile(sol::environment env, const std::string& file)
    {
        auto result = m_state.safe_script_file(file, env, &sol::script_pass_on_error);

        if (!result.valid())
        {
            const sol::error err = result;
            const sol::call_status status = result.status();
            std::ostringstream oss;
            oss << "Lua script execution error: " << err.what() << " (status: " << getStatusString(status) << ")";
            const std::string errorMsg = oss.str();

            VK_ERROR(errorMsg);
        }

        // TODO in future might want to return the result of the script
        // return result;
    }

    void ScriptEngine::RunFunction(sol::environment env, const std::string& funcName, bool ignoreMissing,
                                   const std::vector<LuaType>& args)
    {
        // Get the function
        sol::function func = env[funcName];

        // Check if it's callable
        if (!func.valid())
        {
            if (ignoreMissing)
                return;

            std::ostringstream oss;
            oss << "Lua script execution error: Function '" << funcName << "' not found";
            std::string errorMsg = oss.str();

            VK_ERROR(errorMsg);

            return;
        }

        // Call the function with args, and check for errors
        //  TODO look into sol::variadic_args
        if (sol::protected_function_result result = func(args); !result.valid())
        {
            sol::error err = result;
            sol::call_status status = result.status();

            std::ostringstream oss;
            oss << "Lua script execution error: " << err.what() << " (status: " << getStatusString(status) << ")";
            std::string errorMsg = oss.str();

            //! assumes we always want to know the error of a function
            VK_ERROR(errorMsg);
        }

        // TODO return value of function??
    }

    void ScriptEngine::RegisterTypes()
    {
        RegisterTime(m_state);
        RegisterMath(m_state);
        RegisterInput(m_state);
        RegisterModel(m_state);
        RegisterMesh(m_state);
        RegisterMaterial(m_state);
        RegisterShader(m_state);
        RegisterEntity(m_state);
        RegisterTransform(m_state);
        RegisterTerrain(m_state);
        RegisterFSM(m_state);
        RegisterCamera(m_state);
        RegisterScene(m_state, *this);
        RegisterGUIWindow(m_state);
        RegisterRigidBody(m_state);
        RegisterCollider(m_state);
        RegisterPhysics(m_state);
    }

    void ScriptEngine::RegisterFunctions()
    {
        RegisterAssetLoader(m_state);
        RegisterLogger(m_state);
    }

}; // namespace Vakol
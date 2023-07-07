#include "ScriptEngine.hpp"

#include "Controller/Logger.hpp"

static std::string getStatusString(const sol::call_status status) {
    switch (status) {
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

namespace Vakol {

    ScriptEngine::ScriptEngine(Controller::Time& time) : m_state(), m_time(time) {
        m_state.open_libraries(sol::lib::base);
        m_state.open_libraries(sol::lib::math);
        m_state.open_libraries(sol::lib::string);

        this->RegisterFunctions();
        // TODO - register global vars
        this->RegisterVars();

        CreateScript("scripts/test.js");
        CreateScript("scripts/test1.js");

        // std::vector<JSType> args = {123, "hello", true, 3.14};
        // this->RunFunction("testArgs", args);
    }

    LuaScript ScriptEngine::CreateScript(const std::string& scriptPath) {
        LuaScript script;

        script.path = scriptPath;

        //! please note this may not work as environemnts aren't deep copied (might need to return a luaScript reference
        //! instead)
        script.env = sol::environment(m_state, sol::create, m_state.globals());

        //! run using the scripts own context, (you probably dont want to run every script on creation tho?)
        this->RunFile(script.env, scriptPath);

        // TODO -  remove this code, only have it for testing
        // JSType age = this->GetScriptVariable(script, "age");
        // if (std::holds_alternative<int>(age)) VK_INFO("Age: {0}", std::get<double>(age));

        // Store the new context so that we can destroy it later
        //! this might be doubling handling, may not need to store a list of contexts
        // m_scriptCtxs.push_back(script.env_ctx);

        m_scripts.push_back(script);  //! just for testing - remove later

        return script;
    }
    LuaType ScriptEngine::GetGlobal(const std::string& name) { return GetVariable(m_state.globals(), name); }

    void ScriptEngine::SetGlobal(const std::string& varName, const LuaType& value) {
        m_state.globals()[varName] = value;
    }

    LuaType ScriptEngine::GetScriptVariable(const LuaScript& script, const std::string& varName) {
        return GetVariable(script.env, varName);
    }

    void ScriptEngine::SetScriptVariable(const LuaScript& script, const std::string& varName, const LuaType& value) {
        script.env[varName] = value;
    }

    LuaType ScriptEngine::GetVariable(sol::environment env, const std::string& varName) {
        sol::object var = env[varName];

        if (!var.valid()) {
            std::ostringstream oss;
            oss << "Lua script execution error: Variable '" << varName << "' not found";
            std::string errorMsg = oss.str();

            VK_ERROR(errorMsg);
            return LuaType();
        }

        return var;
    }

    void ScriptEngine::RunFile(sol::environment env, const std::string& file) {
        auto result = m_state.safe_script_file(file, env, &sol::script_pass_on_error);

        if (!result.valid()) {
            const sol::error err = result;
            const sol::call_status status = result.status();
            std::ostringstream oss;
            oss << "Lua script execution error: " << err.what() << " (status: " << getStatusString(status) << ")";
            const std::string errorMsg = oss.str();

            VK_ERROR(errorMsg);
        }

        //! todo in future might want to return the result of the script
        // return result;
    }

    void ScriptEngine::RunFunction(sol::environment env, const std::string& funcName,
                                   const std::vector<LuaType>& args) {
        // Get the function
        sol::function func = env[funcName];

        // Check if it's callable
        if (!func.valid()) {
            std::ostringstream oss;
            oss << "Lua script execution error: Function '" << funcName << "' not found";
            std::string errorMsg = oss.str();

            VK_ERROR(errorMsg);
            return;
        }

        // Call the function with args, and check for errors
        //! could also use sol::variadic_args
        if (sol::protected_function_result result = func(args); !result.valid()) {
            sol::error err = result;
            sol::call_status status = result.status();

            std::ostringstream oss;
            oss << "Lua script execution error: " << err.what() << " (status: " << getStatusString(status) << ")";
            std::string errorMsg = oss.str();

            VK_ERROR(errorMsg);
        }

        //! TODO pass in return type and return it here if it returns anything
    }
};  // namespace Vakol
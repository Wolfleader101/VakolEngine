#pragma once

#include <duktape.h>

#include <string>
#include <variant>
#include <vector>

namespace Vakol {

    struct SScript {
        duk_context* env_ctx;
        std::string path;
    };

    class ScriptEngine {
       public:
        using JSArg = std::variant<int, double, bool, std::string>;
        ScriptEngine();
        ~ScriptEngine();

        SScript CreateScript(const std::string& scriptPath);

        void RunFile(const std::string& file);

        void RunFunction(const std::string& funcName, const std::vector<JSArg>& args);

       private:
        duk_context* m_ctx;
        std::vector<duk_context*> m_scriptCtxs;

        void RegisterFunctions();
    };

}  // namespace Vakol
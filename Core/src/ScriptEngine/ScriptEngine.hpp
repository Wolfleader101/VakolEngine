#pragma once

#include <duktape.h>

#include <string>
#include <variant>
#include <vector>

namespace Vakol {

    class ScriptEngine {
       public:
        using JSArg = std::variant<int, double, bool, std::string>;
        ScriptEngine();
        ~ScriptEngine();

        void RunFile(const std::string& file);  // run a file?

        void RunFunction(const std::string& funcName, const std::vector<JSArg>& args);  // run a function by name?

       private:
        duk_context* m_ctx;

        void RegisterFunctions();
    };

}  // namespace Vakol
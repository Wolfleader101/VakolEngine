#pragma once

#include <duktape.h>

#include <string>

namespace Vakol {

    class ScriptEngine {
       public:
        ScriptEngine();
        ~ScriptEngine();

        void LoadScript(const std::string& script);  // load it into registry?

        void RunFile(const std::string& file);  // run a file?

        void RunFunction(const std::string& funcName);  // run a function by name?

       private:
        duk_context* m_ctx;
    };

}  // namespace Vakol
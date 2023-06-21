add_subdirectory(Model)
add_subdirectory(View)
add_subdirectory(Controller)
add_subdirectory(ScriptEngine)

SET(SOURCES
${MODEL}
${VIEW}
${CONTROLLER}
${SCRIPT_ENGINE}
src/core.cpp
PARENT_SCOPE)
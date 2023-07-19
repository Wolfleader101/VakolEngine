add_subdirectory(Model)
add_subdirectory(View)
add_subdirectory(Controller)
add_subdirectory(Scripting)

SET(SOURCES
${MODEL}
${VIEW}
${CONTROLLER}
${SCRIPTING}
src/core.cpp
PARENT_SCOPE)
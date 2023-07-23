add_subdirectory(Model)
add_subdirectory(View)
add_subdirectory(Controller)
add_subdirectory(Scripting)
add_subdirectory(Math)

SET(SOURCES
${MODEL}
${VIEW}
${CONTROLLER}
${SCRIPTING}
${MATH}
src/core.cpp
PARENT_SCOPE)
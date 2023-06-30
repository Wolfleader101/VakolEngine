add_subdirectory(Model)
add_subdirectory(View)
add_subdirectory(Controller)
add_subdirectory(Math)

SET(SOURCES
${MODEL}
${VIEW}
${CONTROLLER}
${MATH}
src/core.cpp
PARENT_SCOPE)
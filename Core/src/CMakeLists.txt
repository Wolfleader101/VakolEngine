add_subdirectory(Model)
add_subdirectory(View)
add_subdirectory(Controller)

SET(SOURCES
${MODEL}
${VIEW}
${CONTROLLER}
src/core.hpp
src/core.cpp
PARENT_SCOPE)
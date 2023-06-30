add_subdirectory(Model)
add_subdirectory(View)
add_subdirectory(Controller)
add_subdirectory(Math)
add_subdirectory(PhysicsEngine)

SET(SOURCES
${MODEL}
${VIEW}
${CONTROLLER}
${MATH}
${PHYSICS_ENGINE}
src/core.cpp
PARENT_SCOPE)
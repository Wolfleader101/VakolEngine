add_subdirectory(Model)
add_subdirectory(View)
add_subdirectory(Controller)
add_subdirectory(Scripting)
add_subdirectory(Rendering)
add_subdirectory(Math)
add_subdirectory(SceneManager)

SET(SOURCES
${MODEL}
${VIEW}
${CONTROLLER}
${SCRIPTING}
${RENDERING}
${MATH}
${SCENE_MANAGER}
src/core.cpp
PARENT_SCOPE)
add_subdirectory(Events)

SET(CONTROLLER
src/Controller/Singleton.hpp

src/Controller/Animator.hpp
src/Controller/Animator.cpp

src/Controller/Camera.hpp
src/Controller/Camera.cpp

src/Controller/Input.hpp
src/Controller/Input.cpp

src/Controller/Terrain.hpp
src/Controller/Terrain.cpp

src/Controller/SolSerialize.hpp
src/Controller/SolSerialize.cpp
${EVENTS}

PARENT_SCOPE)
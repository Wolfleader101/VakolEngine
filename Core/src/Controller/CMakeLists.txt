add_subdirectory(AssetLoader)
add_subdirectory(Events)
add_subdirectory(Physics)

SET(CONTROLLER


src/Controller/Singleton.hpp

src/Controller/Application.hpp
src/Controller/Application.cpp

src/Controller/Animator.hpp
src/Controller/Animator.cpp

src/Controller/Logger.hpp
src/Controller/Logger.cpp

src/Controller/Time.hpp

src/Controller/Math.hpp

src/Controller/Camera.hpp
src/Controller/Camera.cpp

src/Controller/EntityList.hpp
src/Controller/EntityList.cpp

src/Controller/System.hpp
src/Controller/System.cpp


src/Controller/Scene.hpp
src/Controller/Scene.cpp

src/Controller/SceneManager.hpp
#src/Controller/SceneManager.cpp

src/Controller/Input.hpp
src/Controller/Input.cpp

src/Controller/Terrain.hpp
src/Controller/Terrain.cpp

src/Controller/SolSerialize.hpp
src/Controller/SolSerialize.cpp



${PHYSICS}
${EVENTS}
${ASSETLOADER}

PARENT_SCOPE)
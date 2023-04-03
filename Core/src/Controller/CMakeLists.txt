add_subdirectory(AssetLoader)
add_subdirectory(IO)
add_subdirectory(Serialization)

SET(CONTROLLER
src/Controller/Singleton.hpp

src/Controller/Application.hpp
src/Controller/Application.cpp

src/Controller/LuaState.hpp
src/Controller/LuaState.cpp

src/Controller/Logger.hpp
src/Controller/Logger.cpp

src/Controller/Time.hpp

src/Controller/Camera.hpp
src/Controller/Camera.cpp

src/Controller/EntityList.hpp
src/Controller/EntityList.cpp

src/Controller/System.hpp
src/Controller/System.cpp

src/Controller/LuaAccess.hpp
src/Controller/LuaAccess.cpp

src/Controller/Scene.hpp
src/Controller/Scene.cpp


${SERIALIZABLE}
${EVENTS}
${ASSETLOADER}

PARENT_SCOPE)
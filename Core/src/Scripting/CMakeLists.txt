SET(SCRIPTING_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/ScriptEngine.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/RegisterAssets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RegisterCamera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RegisterComponents.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RegisterECS.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RegisterGUI.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RegisterInput.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RegisterLogger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RegisterMath.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RegisterPhysics.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RegisterPrimitives.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RegisterScene.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RegisterTime.cpp

    PARENT_SCOPE
)

SET(SCRIPTING_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PARENT_SCOPE
)

add_subdirectory(Platform)

SET(RENDERING_SRC
    ${PLATFORM}    
    ${ASSETS}

    ${CMAKE_CURRENT_SOURCE_DIR}/RenderAPI.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RenderCommand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RenderEngine.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/ModelLibrary.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MaterialLibrary.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ShaderLibrary.cpp

    PARENT_SCOPE
)

SET(RENDERING_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PARENT_SCOPE
)

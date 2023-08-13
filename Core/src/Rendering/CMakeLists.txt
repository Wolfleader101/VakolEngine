add_subdirectory(PLATFORM)
add_subdirectory(ASSETS)


set(RENDERING 

${ASSETS}
${PLATFORM}

src/Rendering/RenderAPI.hpp
src/Rendering/RenderAPI.cpp

src/Rendering/RenderEngine.hpp
src/Rendering/RenderEngine.cpp

src/Rendering/RenderCommand.hpp

src/Rendering/RenderData.hpp

src/Rendering/RenderQueue.hpp

src/Rendering/ShaderLibrary.hpp
src/Rendering/ShaderLibrary.cpp

src/Rendering/MaterialLibrary.hpp
src/Rendering/MaterialLibrary.cpp

PARENT_SCOPE)
SET(RENDERING_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/GLRenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RendererFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DebugRenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Instance.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Skybox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/VertexArray.cpp
    PARENT_SCOPE
)

SET(RENDERING_INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PARENT_SCOPE
)

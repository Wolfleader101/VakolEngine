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
src/Rendering/RenderCommand.cpp

src/Rendering/RenderData.hpp

src/Rendering/ShaderLibrary.hpp
src/Rendering/ShaderLibrary.cpp

src/Rendering/MaterialLibrary.hpp
src/Rendering/MaterialLibrary.cpp

PARENT_SCOPE)
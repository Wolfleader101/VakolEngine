add_subdirectory(IMPORTER)

set(ASSETS 

${IMPORTER}

src/Rendering/Assets/Model.hpp
src/Rendering/Assets/Mesh.hpp
src/Rendering/Assets/Camera.hpp
src/Rendering/Assets/Texture.hpp
src/Rendering/Assets/Material.hpp
src/Rendering/Assets/Animation.hpp
src/Rendering/Assets/Shader.hpp

src/Rendering/Assets/ModelLibrary.hpp
src/Rendering/Assets/ModelLibrary.cpp

PARENT_SCOPE)
add_subdirectory(Assets)

SET(MODEL

${ASSETS}

src/Model/Buffer.hpp
src/Model/Buffer.cpp

src/Model/Components.hpp
src/Model/Components.cpp

src/Model/GameConfig.hpp

src/Model/Instance.hpp
src/Model/Instance.cpp

src/Model/Shader.hpp
src/Model/Shader.cpp

src/Model/Skybox.hpp
src/Model/Skybox.cpp

src/Model/VertexArray.hpp
src/Model/VertexArray.cpp
PARENT_SCOPE)

add_subdirectory(Assets)

SET(MODEL

${ASSETS}

#ECS
src/Model/Components.hpp
src/Model/Components.cpp

src/Model/Entity.hpp
src/Model/Entity.cpp

src/Model/GameConfig.hpp

src/Model/Buffer.hpp
src/Model/Buffer.cpp

src/Model/VertexArray.hpp
src/Model/VertexArray.cpp

src/Model/Skybox.hpp

src/Model/Shader.hpp
src/Model/Shader.cpp

src/Model/Instance.hpp
src/Model/Instance.cpp

PARENT_SCOPE)

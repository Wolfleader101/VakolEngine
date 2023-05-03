add_subdirectory(Assets)
add_subdirectory(gl)

SET(MODEL

#OpenGL
${GL}

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

PARENT_SCOPE)

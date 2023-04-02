SET(MODEL

#OpenGL
${GL}

#Buffer
src/Model/Buffer.hpp

#Shader
src/Model/Shader.hpp
src/Model/Shader.cpp

#Math
src/Model/Math/Math.hpp

src/Model/Math/Vector3.hpp
src/Model/Math/Vector3.cpp

src/Model/Math/Vector2.hpp
src/Model/Math/Vector2.cpp

#ECS
src/Model/Components.hpp
src/Model/Components.cpp

src/Model/Entity.hpp
src/Model/Entity.cpp

src/Model/GameConfig.hpp

${ASSETS}

PARENT_SCOPE)

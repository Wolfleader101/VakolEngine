#include "include/Instance.hpp"

#include "Math/include/Math.hpp"
#include <glad/glad.h>

namespace Vakol
{
    void CreateInstances(const std::vector<Mesh>& meshes, const std::vector<Math::Mat4>& matrices, const int amount,
                         const int start_index)
    {
        unsigned int buffer;

        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(matrices.size() * sizeof(Math::Mat4)), matrices.data(),
                     GL_STATIC_DRAW);

        for (const auto& mesh : meshes)
        {
            mesh.SetDrawMode(DRAW_MODE::INSTANCED);
            mesh.SetDrawModeInfo(amount);
            mesh.SetDrawType(DRAW_TYPE::ELEMENTS);

            const unsigned int vao = mesh.GetId();
            glBindVertexArray(vao);

            // mat4 is a 4x4 matrix (4 * vec4), which in our case, represents the transformation matrix of a model.
            // transformation matrix containing the translation, scaling, and rotation of a model.

            // since the max size of an attribute in glsl shaders is a vec4,
            // we need to reserve enough space for a mat4

            // sizeof(mat4) = 4 * sizeof(vec4) = 16 * sizeof(float)

            // vec4 is a 4x1 vector representing 4 floats

            // set the attribute location to 3, reserving 4 GL_FLOATS in the vertex array

            glEnableVertexAttribArray(start_index);
            glVertexAttribPointer(start_index, 4, GL_FLOAT, GL_FALSE, sizeof(Math::Mat4), nullptr);

            glEnableVertexAttribArray(start_index + 1);
            glVertexAttribPointer(start_index + 1, 4, GL_FLOAT, GL_FALSE, sizeof(Math::Mat4),
                                  reinterpret_cast<void*>(sizeof(Math::Vec4)));

            glEnableVertexAttribArray(start_index + 2);
            glVertexAttribPointer(start_index + 2, 4, GL_FLOAT, GL_FALSE, sizeof(Math::Mat4),
                                  reinterpret_cast<void*>(2 * sizeof(Math::Vec4)));

            glEnableVertexAttribArray(start_index + 3);
            glVertexAttribPointer(start_index + 3, 4, GL_FLOAT, GL_FALSE, sizeof(Math::Mat4),
                                  reinterpret_cast<void*>(3 * sizeof(Math::Vec4)));

            glVertexAttribDivisor(start_index, 1);
            glVertexAttribDivisor(start_index + 1, 1);
            glVertexAttribDivisor(start_index + 2, 1);
            glVertexAttribDivisor(start_index + 3, 1);

            glBindVertexArray(0);
        }
    }
} // namespace Vakol
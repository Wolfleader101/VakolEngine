#include "Instance.hpp"

#include <glad/glad.h>

namespace Vakol::Model
{
    void CreateInstances(const std::vector<Mesh>& meshes, const std::vector<glm::mat4>& matrices, const int amount)
    {
        unsigned int buffer;

        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(matrices.size() * sizeof(glm::mat4)), matrices.data(), GL_STATIC_DRAW);

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

            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), nullptr);

            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(sizeof(glm::vec4)));

            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(2 * sizeof(glm::vec4)));

            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(3 * sizeof(glm::vec4)));

            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);

            glBindVertexArray(0);
        }
    }
}
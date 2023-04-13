#include "GLRenderer.hpp"

#include <glad/glad.h>

#include <Controller/Logger.hpp>
#include <glm/common.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

#include "Model/gl/GLShader.hpp"

namespace Vakol::View {
    GLRenderer::GLRenderer(const std::shared_ptr<Window> window) : Renderer(window) {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);

        std::vector<Vertex> vertices = {{{glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                                          glm::vec2(1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)},
                                         {glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                                          glm::vec2(1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f)},
                                         {glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                                          glm::vec2(0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f)},
                                         {glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                                          glm::vec2(0.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f)}}};

        std::vector<unsigned int> indices = {0, 1, 3, 1, 2, 3};
        m_vertexArray = std::make_shared<GLVertexArray>(vertices, indices);
        m_shader = std::make_shared<GLShader>("coreAssets/Shaders/basic.prog");

        std::shared_ptr<VertexArray> m_vertexArray = nullptr;
        std::shared_ptr<Model::Assets::Shader> m_shader = nullptr;

        //    std::shared_ptr<VertexArray> m_vertexArray = nullptr;
        // std::shared_ptr<Model::Assets::Shader> m_shader = nullptr;
    };

    // void Draw(const Controller::Time& time, const std::shared_ptr<Model::Components::Drawable> drawable) {}

    void GLRenderer::Update(Controller::Camera& cam) {
        glClearColor(0.52941f, 0.80784f, 0.92157f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_shader->Bind();

        m_shader->SetMat4("_PV", cam.GetMatrix(_PV_MATRIX));

        glBindVertexArray(m_vertexArray->GetID());
        glDrawElements(GL_TRIANGLES, m_vertexArray->GetIndices(), GL_UNSIGNED_INT, 0);

        m_shader->Unbind();
    };
}  // namespace Vakol::View
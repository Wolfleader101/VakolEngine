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
    };

    void GLRenderer::Draw(const Controller::Time& time, const Controller::Camera& camera, const Model::Components::Drawable& drawable) const
    {
        glClearColor(0.52941f, 0.80784f, 0.92157f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (const auto& mesh : drawable.model_ptr->meshes())
        {
            mesh.material()->Bind();
            mesh.material()->SetMat4("_PV", camera.GetMatrix(_PV_MATRIX));

            mesh.vao()->Draw();
        }
    }
}  // namespace Vakol::View
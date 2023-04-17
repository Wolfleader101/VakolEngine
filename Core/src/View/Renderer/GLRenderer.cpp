#include "GLRenderer.hpp"

#include <glad/glad.h>

#include <Controller/Logger.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Model/Components.hpp"
#include "Model/gl/GLShader.hpp"

/*
Distance	Constant	Linear	Quadratic
7	        1.0	        0.7	    1.8
13	        1.0	        0.35	0.44
20	        1.0	        0.22	0.20
32	        1.0	        0.14	0.07
50	        1.0	        0.09	0.032
65	        1.0	        0.07	0.017
100	        1.0	        0.045	0.0075
160	        1.0	        0.027	0.0028
200	        1.0	        0.022	0.0019
325	        1.0	        0.014	0.0007
600	        1.0	        0.007	0.0002
3250	    1.0	        0.0014	0.000007
*/

namespace Vakol::View {
    GLRenderer::GLRenderer(const std::shared_ptr<Window> window) : Renderer(window) {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
    };

    void GLRenderer::Draw(const Controller::Time& time, const Controller::Camera& camera,
                          const Model::Components::Transform trans, const Model::Components::Drawable& drawable) const {
        glm::mat4 model_matrix = glm::mat4(1.0f);

        model_matrix = glm::translate(model_matrix, trans.pos);

        model_matrix = glm::scale(model_matrix, trans.scale);

        model_matrix = glm::rotate(model_matrix, trans.rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model_matrix = glm::rotate(model_matrix, trans.rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model_matrix = glm::rotate(model_matrix, trans.rot.z, glm::vec3(0.0f, 0.0f, 1.0f));

        for (auto mesh : drawable.model_ptr->meshes()) {
            mesh.material()->SetMat4("PV", camera.GetMatrix(_PV_MATRIX));
            mesh.material()->SetMat4("MODEL", model_matrix);

            // mesh.vao()->Bind();
            // glDrawElementsInstanced(GL_TRIANGLES, mesh.vao()->GetIndices(), GL_UNSIGNED_INT, 0, 30000);
            // mesh.vao()->Unbind();

            if (mesh.IsStripped()) {
                mesh.vao()->DrawElementsStripped(mesh.strips);
            } else {
                mesh.vao()->DrawElements();
            }
        }
    }

    void GLRenderer::Update() const {
        glClearColor(0.52941f, 0.80784f, 0.92157f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}  // namespace Vakol::View
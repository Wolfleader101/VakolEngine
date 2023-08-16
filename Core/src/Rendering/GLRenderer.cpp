#include "Rendering/GLRenderer.hpp"

#include <glad/glad.h>
#include <memory>
#include <vector>

#include "ECS/Components.hpp"
#include "Logger/Logger.hpp"

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

using namespace Vakol;

constexpr Math::Vec4 VAKOL_CLASSIC = Math::Vec4(0.52941f, 0.80784f, 0.92157f, 1.0f);
constexpr Math::Vec4 VAKOL_FOGGY = Math::Vec4(0.4f, 0.4f, 0.4f, 1.0);
constexpr Math::Vec4 VAKOL_FOGGY_2 = Math::Vec4(0.8f, 0.8f, 0.8f, 0.0);
constexpr Math::Vec4 VAKOL_DARK = Math::Vec4(0.1f, 0.1f, 0.1f, 1.0f);

constexpr float LIGHT_CONSTANT = 1.0f;
constexpr float LIGHT_LINEAR = 0.045f;
constexpr float LIGHT_QUADRATIC = 0.0075f;

const float LIGHT_CUT_OFF = Math::Cos(Math::DegToRad(7.5f));
const float LIGHT_OUTER_CUT_OFF = Math::Cos(Math::DegToRad(12.5f));

Math::Mat4 PROJECTION = Math::Mat4(1.0f);
Math::Mat4 VIEW = Math::Mat4(1.0f);

namespace Vakol
{
    GLRenderer::GLRenderer(const std::shared_ptr<Window>& window) : Renderer(window)
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // this corresponds to the uniform buffer in each shader that has one.
        // layout (std140, binding = 1) uniform <name>
        // std140 - memory layout, binding - index, uniform (typeof buffer)
        // add a uniform buffer which size is that of a 4x4 matrix with a binding index of 1
        AddBuffer(GL_UNIFORM_BUFFER, 3 * sizeof(Math::Mat4), 1, GL_STATIC_DRAW);
        AddBuffer(GL_UNIFORM_BUFFER, 5 * sizeof(float), 2, GL_STATIC_DRAW);

        SetBufferSubData(1, 0, sizeof(float), &LIGHT_CONSTANT);
        SetBufferSubData(1, 1 * sizeof(float), sizeof(float), &LIGHT_LINEAR);
        SetBufferSubData(1, 2 * sizeof(float), sizeof(float), &LIGHT_QUADRATIC);
        SetBufferSubData(1, 3 * sizeof(float), sizeof(float), &LIGHT_CUT_OFF);
        SetBufferSubData(1, 4 * sizeof(float), sizeof(float), &LIGHT_OUTER_CUT_OFF);

        // ATTACHMENT attachment{ {GL_COLOR_ATTACHMENT0, GL_RGB8, GL_RGB, 800, 600}, {GL_DEPTH_STENCIL_ATTACHMENT,
        // GL_DEPTH24_STENCIL8} };

        // framebuffers.push_back(std::make_shared<FrameBuffer>(attachment, true));

        if (isSkybox)
            skybox->Init();
    }

    void GLRenderer::AddBuffer(const unsigned int type, const int size, const int binding, const void* data,
                               const unsigned int usage)
    {
        buffers.push_back(std::make_shared<Buffer>(type, size, binding, data, usage));
    }

    void GLRenderer::AddBuffer(const unsigned int type, const int size, const int binding, const unsigned int usage)
    {
        buffers.push_back(std::make_shared<Buffer>(type, size, binding, usage));
    }

    void GLRenderer::SetBufferSubData(const int index, const int offset, const int size, const void* data) const
    {
        buffers.at(index)->SetSubData(offset, size, data);
    }

    void GLRenderer::ClearColor(const Math::Vec4& color) const
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void GLRenderer::ClearColor(const float r, const float g, const float b, const float a) const
    {
        glClearColor(r, g, b, a);
    }

    void GLRenderer::ClearBuffer(const unsigned int buffer_bit) const
    {
        glClear(buffer_bit);
    }

    void GLRenderer::DrawAnimated(const Components::Transform& transform, const Components::Drawable& drawable,
                                  const Assets::Animation& animation) const
    {
        const auto& model = drawable.model_ptr;
        if (model == nullptr)
        {
            VK_CRITICAL("Model {0} is null", drawable.name);
            return;
        }

        const auto& shader = model->c_shader();

        if (shader == nullptr)
        {
            VK_CRITICAL("Shader for {0} is null", drawable.name);
            return;
        }

        if (!model->cullBackface())
            glDisable(GL_CULL_FACE);

        const auto translation_matrix = translate(Math::Mat4(1.0f), transform.pos);

        const auto rotation_matrix = mat4_cast(transform.rot);

        const auto scale_matrix = scale(Math::Mat4(1.0f), transform.scale);

        shader->SetMat4("MODEL_MATRIX", translation_matrix * rotation_matrix * scale_matrix);

        shader->SetMat4v("BONE_TRANSFORMS", animation.numTransforms(), Math::AsArray(animation.transform()));

        shader->Bind();
        for (int i = 0; i < model->nMeshes(); ++i)
        {
            const auto& mesh = model->mesh(i);
            const auto& material = mesh.GetMaterial();

            for (int j = 0; j < material->GetTextureCount(); ++j)
            {
                glActiveTexture(GL_TEXTURE0 + j);
                glBindTexture(GL_TEXTURE_2D, material->GetTexture(j));
            }

            mesh.Draw();
        }

        shader->Unbind();
    }

    void GLRenderer::Draw(const Components::Transform& transform, const Components::Drawable& drawable) const
    {
        const auto& model = drawable.model_ptr;

        if (model == nullptr)
        {
            VK_CRITICAL("Model {0} is null", drawable.name);
            return;
        }

        const auto& shader = model->c_shader();

        if (shader == nullptr)
        {
            VK_CRITICAL("Shader for {0} is null", drawable.name);
            return;
        }

        if (!model->cullBackface())
            glDisable(GL_CULL_FACE);

        const auto translation_matrix = translate(Math::Mat4(1.0f), transform.pos);

        const auto rotation_matrix = mat4_cast(transform.rot);

        const auto scale_matrix = scale(Math::Mat4(1.0f), transform.scale);

        shader->SetMat4("MODEL_MATRIX", translation_matrix * rotation_matrix * scale_matrix);

        shader->Bind();

        for (int i = 0; i < model->nMeshes(); ++i)
        {
            const auto& mesh = model->mesh(i);
            const auto& material = mesh.GetMaterial();

            for (int j = 0; j < material->GetTextureCount(); ++j)
            {
                glActiveTexture(GL_TEXTURE0 + j);
                glBindTexture(GL_TEXTURE_2D, material->GetTexture(j));
            }

            mesh.Draw();
        }

        shader->Unbind();
    }

    void GLRenderer::UpdateData(const Camera& camera)
    {
        PROJECTION = camera.GetMatrix(PROJECTION_MATRIX);
        VIEW = camera.GetMatrix(VIEW_MATRIX);

        // at index 0, with an offset of 0 (since PV_MATRIX is the only element in the buffer), with a size of a 4x4
        // matrix, set PV_MATRIX
        SetBufferSubData(0, 0, sizeof(Math::Mat4), Math::AsArray(PROJECTION));
        SetBufferSubData(0, sizeof(Math::Mat4), sizeof(Math::Mat4), Math::AsArray(VIEW));
        SetBufferSubData(0, 2 * sizeof(Math::Mat4), sizeof(Math::Mat4), Math::AsArray(PROJECTION * VIEW));
    }

    void GLRenderer::Update([[maybe_unused]] const int index) const
    {
        ClearColor(VAKOL_CLASSIC);
        ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // if (index > -1)
        //{
        //     framebuffers.at(index)->ClearColor(VAKOL_DARK);

        //    if (framebuffers.at(index)->HasDepth())
        //        framebuffers.at(index)->ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //    else
        //        framebuffers.at(index)->ClearBuffer(GL_COLOR_BUFFER_BIT);
        //}
    }

    void GLRenderer::LateUpdate([[maybe_unused]] const int index) const
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        if (isSkybox)
            skybox->Draw(PROJECTION, VIEW);
    }
} // namespace Vakol
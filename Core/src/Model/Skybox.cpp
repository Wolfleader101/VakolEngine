#include "Skybox.hpp"

#include <vector>

#include <glad/glad.h>

#include <Controller/AssetLoader/TextureLoader.hpp>

// Keep in this order
std::vector skybox_faces = { "assets/textures/skybox/Stolen/right.jpg", "assets/textures/skybox/Stolen/left.jpg",
							 "assets/textures/skybox/Stolen/top.jpg",   "assets/textures/skybox/Stolen/bottom.jpg",
							 "assets/textures/skybox/Stolen/front.jpg", "assets/textures/skybox/Stolen/back.jpg" };

float skybox_vertices[] =
{
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

unsigned int SKYBOX_VAO, SKYBOX_VBO;

namespace Vakol::Model
{
	void Skybox::Init()
	{
		shader = std::make_shared<Shader>("coreAssets/shaders/skybox.prog");

        glGenVertexArrays(1, &SKYBOX_VAO);
        glGenBuffers(1, &SKYBOX_VBO);

        glBindVertexArray(SKYBOX_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, SKYBOX_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        CUBEMAP_ID = Controller::LoadTexture(std::move(skybox_faces), false, false);
	}

    void Skybox::Draw(const glm::mat4& projection, const glm::mat4& view) const
	{
        glDepthFunc(GL_LEQUAL);

        shader->Bind();

        const auto t_view = glm::mat4(glm::mat3(view)); // remove transform matrix
        shader->SetMat4("PV_MATRIX", projection * t_view);

        glBindVertexArray(SKYBOX_VAO);
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_CUBE_MAP, CUBEMAP_ID);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        glDepthFunc(GL_LESS);

        shader->Unbind();
	}
	
}
#pragma once

#include <memory>

#include <Model/Shader.hpp>

namespace Vakol::Model
{
    /**
     * @brief Struct representing a Skybox.
     */
    struct Skybox {
        /**
         * @brief Default constructor for the Skybox struct.
         */
        Skybox() = default;

        /**
         * @brief Function to initialize the skybox.
         */
        void Init();

        /**
         * @brief Function to draw the skybox.
         * @param projection Projection matrix.
         * @param view View matrix.
         */
        void Draw(const glm::mat4& projection, const glm::mat4& view) const;

       private:
        std::shared_ptr<Shader> shader = nullptr;  ///< Pointer to the shader.
        unsigned int CUBEMAP_ID = 0;               ///< ID of the cubemap texture.
    };
}
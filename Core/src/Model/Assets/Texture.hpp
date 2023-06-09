#pragma once

#include <string>

namespace Vakol::Model::Assets 
{   
    /**
     * @brief Class representing a texture.
     */
    class Texture {
       public:
        Texture() = default;

        /**
         * @brief Constructs a Texture object with the given path.
         *
         * @param path The path to the texture.
         */
        explicit Texture(std::string&& path) : path(std::move(path)) {}

        /**
         * @brief Constructs a Texture object with the given path.
         *
         * @param path The path to the texture.
         */
        explicit Texture(std::string& path) : path(std::move(path)) {}

        /**
         * @brief Sets the ID of the texture.
         *
         * @param id The ID of the texture.
         */
        void SetID(const unsigned int id) { this->m_ID = id; }

        /**
         * @brief Gets the ID of the texture.
         *
         * @return The ID of the texture.
         */
        [[nodiscard]] unsigned int GetID() const { return this->m_ID; }

        std::string path; /**< The path to the texture. */

       private:
        unsigned int m_ID = 0; /**< The ID of the texture. */
    };
}
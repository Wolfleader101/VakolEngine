#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

#define USE_TRIANGLE_PATCHES 0

namespace Vakol::Model {
    constexpr int MAX_BONE_INFLUENCE = 4;

#if USE_TRIANGLE_PATCHES
    constexpr int NUM_PATCH_PTS = 3;
#else
    constexpr int NUM_PATCH_PTS = 4;
#endif

    /**
     * @brief Struct representing a Vertex.
     */
    struct Vertex {
        glm::vec3 position;   ///< Position of the vertex.
        glm::vec3 normal;     ///< Normal of the vertex.
        glm::vec2 uv;         ///< Texture coordinates of the vertex.
        glm::vec3 tangent;    ///< Tangent of the vertex.
        glm::vec3 bitangent;  ///< Bitangent of the vertex.

        int bone_ids[MAX_BONE_INFLUENCE];        ///< Array of bone IDs affecting the vertex.
        float bone_weights[MAX_BONE_INFLUENCE];  ///< Array of weights of the bones affecting the vertex.
    };

    /**
     * @brief Enum representing the method of drawing.
     */
    enum class DRAW_MODE {
        DEFAULT,    ///< Default drawing mode.
        INSTANCED,  ///< Instanced drawing mode.
        STRIPS,     ///< Strips drawing mode.
        PATCHES     ///< Patches drawing mode.
    };

    /**
     * @brief Enum representing the type of drawing.
     */
    enum class DRAW_TYPE {
        ARRAYS,   ///< Draw with vertices only (arrays).
        ELEMENTS  ///< Draw using indices (elements).
    };

   /**
     * @brief Struct representing draw information.
     */
    struct DrawInfo {
        DRAW_MODE draw_mode = DRAW_MODE::DEFAULT;   ///< Method of drawing.
        DRAW_TYPE draw_type = DRAW_TYPE::ELEMENTS;  ///< Type of drawing.

        int INSTANCE_AMOUNT = 0;  ///< Number of instances for instanced drawing.

        int NUM_STRIPS = 0;  ///< Number of strips for strip drawing.

        int NUM_TRIS_PER_STRIP = 0;  ///< Number of triangles per strip for strip drawing.

        int NUM_PATCHES = 0;                            ///< Number of patches for patch drawing.
        const int NUM_VERTS_PER_PATCH = NUM_PATCH_PTS;  ///< Number of vertices per patch for patch drawing.
    };

    /**
     * @brief Function to convert a vector of floats to a vector of Vertex.
     * @param arr Array of floats.
     * @param size Size of the array.
     * @return Vector of Vertex.
     */
    std::vector<Vertex> Convert(std::vector<float>& arr, size_t size);

   /**
     * @brief Class representing a Vertex Array Object (VAO).
     */
    class VertexArray {
       public:
        /**
         * @brief Constructs a new Vertex Array.
         * @param vertices A vector of vertices.
         * @param indices A vector of indices.
         * @param size Size of the array.
         */
        VertexArray(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, size_t size);

        /**
         * @brief Destructs the Vertex Array.
         */
        ~VertexArray();

        /**
         * @brief Draws the Vertex Array.
         */
        void Draw() const;

        /**
         * @brief Generates Vertex Array(s).
         * @param n Number of arrays to generate.
         * @param array Pointer to the array.
         */
        static void GenArray(unsigned int n, unsigned int* array);

        /**
         * @brief Specify the data of a vertex attribute.
         * @param index The location at which this vertex attribute occurs.
         * @param n Number of elements for which the attribute contains.
         * @param type The type of data to be used.
         * @param normalized Whether to normalize the data.
         * @param stride Byte offset between each consecutive vertex.
         * @param data The byte offset from the starting vertex attribute.
         */
        static void SetVertexAttributeData(unsigned int index, int n, unsigned int type, bool normalized, size_t stride,
                                           const void* data);
        static void SetVertexAttributeIData(unsigned int index, int n, unsigned int type, size_t stride,
                                            const void* data);

        /**
         * @brief Binds the Vertex Array.
         */
        void Bind() const;

        /**
         * @brief Unbinds the Vertex Array.
         */
        static void Unbind();

        /**
         * @brief Gets the Vertex Array's ID.
         * @return ID of the Vertex Array.
         */
        [[nodiscard]] unsigned int GetId() const { return this->ID; }

        /**
         * @brief Gets the Vertex Count.
         * @return Number of vertices.
         */
        [[nodiscard]] int GetVertexCount() const { return this->n_vertices; }

        /**
         * @brief Gets the Index Count.
         * @return Number of indices.
         */
        [[nodiscard]] int GetIndexCount() const { return this->n_indices; }

        /**
         * @brief Sets the vertices.
         * @param in_vertices Vector of vertices.
         */
        void set(std::vector<Vertex>& in_vertices) { this->vertices = std::move(in_vertices); }

        /**
         * @brief Sets the indices.
         * @param in_indices Vector of indices.
         */
        void set(std::vector<unsigned int>& in_indices) { this->indices = std::move(in_indices); }

        [[nodiscard]] const std::vector<Vertex>& GetConstVertices() const { return this->vertices; }
        [[nodiscard]] std::vector<Vertex>& GetVertices() { return this->vertices; }

        [[nodiscard]] const std::vector<unsigned int>& GetConstIndices() const { return this->indices; }
        [[nodiscard]] std::vector<unsigned int>& GetIndices() { return this->indices; }

        void set_mode(const DRAW_MODE mode) { this->info.draw_mode = mode; }
        void set_type(const DRAW_TYPE type) { this->info.draw_type = type; }

        void set_mode_data(const int data) {
            if (this->info.draw_mode == DRAW_MODE::INSTANCED) this->info.INSTANCE_AMOUNT = data;

            if (this->info.draw_mode == DRAW_MODE::STRIPS) this->info.NUM_STRIPS = data;

            if (this->info.draw_mode == DRAW_MODE::PATCHES) this->info.NUM_PATCHES = data;
        }

        void set_num_tris_per_strip(const int data) {
            if (this->info.draw_mode == DRAW_MODE::STRIPS) this->info.NUM_TRIS_PER_STRIP = data;
        }

       private:
        unsigned int ID = 0;  ///< Vertex Array ID.

        unsigned int VBO = 0;  ///< Vertex Buffer Object ID.
        unsigned int EBO = 0;  ///< Element Buffer Object ID.

        int n_vertices = 0;  ///< Number of vertices.
        int n_indices = 0;   ///< Number of indices.

        DrawInfo info;  ///< Drawing information.

        std::vector<Vertex> vertices;       ///< Vector of vertices.
        std::vector<unsigned int> indices;  ///< Vector of indices.
    };
}  // namespace Vakol::Model
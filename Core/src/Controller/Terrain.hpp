#pragma once

#include <Model/Assets/Model.hpp>
#include <memory>
#include <string>
#include <vector>

namespace Vakol::Controller {

    enum class TerrainType { FAULT, HEIGHT, CLOD }; //for serializing
    /**
     * @class Terrain
     *
     * @brief Class representing terrain in the application.
     */
    class Terrain {
       public:
        Terrain() = default;

        /**
         * @brief Set the terrain model using a mesh.
         *
         * @param mesh The mesh to set.
         */
        void SetModel(Model::Assets::Mesh&& mesh) { this->m_model = std::make_shared<Model::Assets::Model>(mesh); }

        /**
         * @brief Set the terrain model using a model.
         *
         * @param model The model to set.
         */
        void SetModel(Model::Assets::Model&& model) {
            this->m_model = std::make_shared<Model::Assets::Model>(std::move(model));
        }

        /**
         * @brief Get the terrain model.
         *
         * @return The shared pointer to the model.
         */
        const std::shared_ptr<Model::Assets::Model>& GetModel() const { return this->m_model; }

        /**
         * @brief Set the terrain data.
         *
         * @param data The pointer to the terrain data.
         */
        void SetData(const unsigned char* data);

        /**
         * @brief Get the terrain size.
         *
         * @return The size of the terrain.
         */
        int GetSize() const { return this->m_size; }

        /**
         * @brief Set the terrain size.
         *
         * @param size The size of the terrain.
         */
        void SetSize(const int size) { this->m_size = size; }

        /**
         * @brief Get the height at the specified position in the terrain.
         *
         * @param x The x position.
         * @param z The z position.
         * @return The height at the position.
         */
        float GetHeight(float x, float z) const;

        /**
         * @brief Load a height map mesh from the terrain.
         *
         * @return The loaded height map mesh.
         */
        Model::Assets::Mesh load_height_map_mesh() const;

        /**
         * @brief Load a CLOD (Continuous Level of Detail) terrain mesh.
         *
         * @return The loaded CLOD terrain mesh.
         */
        Model::Assets::Mesh load_clod_terrain_mesh() const;

        /**
         * @brief Get the height map data.
         *
         * @return The vector containing the height map.
         */
        const std::vector<float>& GetHeightMap() const { return this->m_height_map; }

        /**
         * @brief Get the terrain data.
         *
         * @return The vector containing the terrain data.
         */
        const std::vector<unsigned char>& GetData() const { return this->m_data; }

        /**
         * @brief Set the minimum and maximum height of the terrain.
         *
         * @param min The minimum height.
         * @param max The maximum height.
         */
        void SetMinMax(const float min, const float max) {
            this->m_min_height = min;
            this->m_max_height = max;
        }

        /**
         * @brief Get the minimum height of the terrain.
         *
         * @return The minimum height.
         */
        [[nodiscard]] float GetMinHeight() const { return static_cast<float>(this->m_min_height); }

        /**
         * @brief Get the maximum height of the terrain.
         *
         * @return The maximum height.
         */
        [[nodiscard]] float GetMaxHeight() const { return static_cast<float>(this->m_max_height); }

       private:
        /**
         * @brief Load a fault formation terrain mesh.
         *
         * @param size The size of the terrain.
         * @param iterations The number of iterations for fault formation.
         * @param filter The FIR filter value.
         * @param random Flag indicating random fault formation.
         * @param minHeight The minimum height for the terrain.
         * @param maxHeight The maximum height for the terrain.
         * @return The loaded fault formation terrain mesh.
         */
        Model::Assets::Mesh load_fault_formation_mesh(int size, int iterations, float filter, bool random,
                                                      int minHeight, int maxHeight);

        /**
         * @struct Point
         *
         * @brief Struct representing a point in the terrain.
         */
        struct Point {
            int x = 0;
            int z = 0;

            /**
             * @brief Equality operator for points.
             *
             * @param other The other point to compare.
             * @return True if the points are equal, false otherwise.
             */
            bool operator==(const Point& other) const { return this->x == other.x && this->z == other.z; }

            /**
             * @brief Subtract operator for points.
             *
             * @param other The other point to subtract.
             * @return The result of the subtraction.
             */
            Point operator-(const Point& other) const { return {this->x - other.x, this->z - other.z}; }
        };

        /**
         * @brief Generate random points for fault formation.
         *
         * @param p1 The first point.
         * @param p2 The second point.
         * @param size The size of the terrain.
         */
        static void GenRandomPoints(Point& p1, Point& p2, const int size);

        /**
         * @brief Normalize values in an array.
         *
         * @param arr The array to normalize.
         * @param size The size of the array.
         */
        void NormalizeValues(std::vector<float>& arr, const int size);

        /**
         * @brief Set the height map of the terrain.
         */
        void SetHeightMap();

        /**
         * @brief Apply an FIR filter to an array.
         *
         * @param arr The array to filter.
         * @param size The size of the array.
         * @param filter The FIR filter value.
         */
        static void ApplyFIRFilter(std::vector<float>& arr, const int size, const float filter);

        /**
         * @brief Perform a single pass of the FIR filter.
         *
         * @param arr The array to filter.
         * @param index The current index in the array.
         * @param prev The previous filtered value.
         * @param filter The FIR filter value.
         * @return The filtered value at the current index.
         */
        static float FirSinglePass(std::vector<float>& arr, const int index, const float prev, const float filter);

        /**
         * @brief The shared pointer to the terrain model.
         */
        std::shared_ptr<Model::Assets::Model> m_model;

        /**
         * @brief The size of the terrain.
         */
        int m_size = 0;

        /**
         * @brief The minimum height of the terrain.
         */
        float m_min_height = 0.0f;

        /**
         * @brief The maximum height of the terrain.
         */
        float m_max_height = 0.0f;

        /**
         * @brief The height map of the terrain.
         */
        std::vector<float> m_height_map;

        /**
         * @brief The terrain data.
         */
        std::vector<unsigned char> m_data;
    };

    /**
     * @brief Load a height map terrain from a file.
     *
     * @param path The path to the height map file.
     * @param min The minimum height of the terrain.
     * @param max The maximum height of the terrain.
     * @return The loaded height map terrain.
     */
    Terrain LoadHeightMapTerrain(std::string&& path, float min, float max);

    /**
     * @brief Load a CLOD (Continuous Level of Detail) terrain from a file.
     *
     * @param path The path to the CLOD terrain file.
     * @param min The minimum height of the terrain.
     * @param max The maximum height of the terrain.
     * @return The loaded CLOD terrain.
     */
    Terrain LoadCLODTerrain(std::string&& path, float min, float max);
}  // namespace Vakol::Controller
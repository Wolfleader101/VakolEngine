#include "Primitives/Cube.hpp"

namespace Vakol
{
    Cube::Cube()
    {
        mesh.name = "DEFAULT_CUBE_mesh"; // Set the name to DEFAULT_CUBE_mesh
        mesh.ID = xg::newGuid().str();   // Generate a new GUID for the mesh

        GenerateData(); // Generate the data for the cube
    }

    Cube::Cube(std::string inputName, std::string inputID)
    {
        mesh.name = inputName + "_mesh"; // Set the name to the input name
        mesh.ID = inputID;               // Generate a new GUID for the mesh

        GenerateData(); // Generate the data for the cube
    }

    void Cube::GenerateData()
    {
        Rendering::Vertex tmpVertex; // A temporary vertex object to store the data

        // Use pre-calculated positions for the cube
        std::vector<Math::Vec3> preCalcPositions = {// Back face
                                                    {-0.5f, -0.5f, -0.5f},
                                                    {0.5f, -0.5f, -0.5f},
                                                    {0.5f, 0.5f, -0.5f},
                                                    {-0.5f, 0.5f, -0.5f},
                                                    // Front face
                                                    {-0.5f, -0.5f, 0.5f},
                                                    {0.5f, -0.5f, 0.5f},
                                                    {0.5f, 0.5f, 0.5f},
                                                    {-0.5f, 0.5f, 0.5f},
                                                    // Left face
                                                    {-0.5f, -0.5f, -0.5f},
                                                    {-0.5f, -0.5f, 0.5f},
                                                    {-0.5f, 0.5f, 0.5f},
                                                    {-0.5f, 0.5f, -0.5f},
                                                    // Right face
                                                    {0.5f, -0.5f, -0.5f},
                                                    {0.5f, -0.5f, 0.5f},
                                                    {0.5f, 0.5f, 0.5f},
                                                    {0.5f, 0.5f, -0.5f},
                                                    // Bottom face
                                                    {-0.5f, -0.5f, -0.5f},
                                                    {0.5f, -0.5f, -0.5f},
                                                    {0.5f, -0.5f, 0.5f},
                                                    {-0.5f, -0.5f, 0.5f},
                                                    // Top face
                                                    {-0.5f, 0.5f, -0.5f},
                                                    {0.5f, 0.5f, -0.5f},
                                                    {0.5f, 0.5f, 0.5f},
                                                    {-0.5f, 0.5f, 0.5f}};

        // Use pre-calculated normals for the cube
        std::vector<Math::Vec3> preCalcNormals = {// Back face
                                                  {0.0f, 0.0f, -1.0f},
                                                  {0.0f, 0.0f, -1.0f},
                                                  {0.0f, 0.0f, -1.0f},
                                                  {0.0f, 0.0f, -1.0f},
                                                  // Front face
                                                  {0.0f, 0.0f, 1.0f},
                                                  {0.0f, 0.0f, 1.0f},
                                                  {0.0f, 0.0f, 1.0f},
                                                  {0.0f, 0.0f, 1.0f},
                                                  // Left face
                                                  {-1.0f, 0.0f, 0.0f},
                                                  {-1.0f, 0.0f, 0.0f},
                                                  {-1.0f, 0.0f, 0.0f},
                                                  {-1.0f, 0.0f, 0.0f},
                                                  // Right face
                                                  {1.0f, 0.0f, 0.0f},
                                                  {1.0f, 0.0f, 0.0f},
                                                  {1.0f, 0.0f, 0.0f},
                                                  {1.0f, 0.0f, 0.0f},
                                                  // Bottom face
                                                  {0.0f, -1.0f, 0.0f},
                                                  {0.0f, -1.0f, 0.0f},
                                                  {0.0f, -1.0f, 0.0f},
                                                  {0.0f, -1.0f, 0.0f},
                                                  // Top face
                                                  {0.0f, 1.0f, 0.0f},
                                                  {0.0f, 1.0f, 0.0f},
                                                  {0.0f, 1.0f, 0.0f},
                                                  {0.0f, 1.0f, 0.0f}};

        // Use pre-calculated UVs for the cube
        std::vector<Math::Vec2> preCalcUVs = {// Back face
                                              {0.0f, 0.0f},
                                              {1.0f, 0.0f},
                                              {1.0f, 1.0f},
                                              {0.0f, 1.0f},
                                              // Front face
                                              {0.0f, 0.0f},
                                              {1.0f, 0.0f},
                                              {1.0f, 1.0f},
                                              {0.0f, 1.0f},
                                              // Left face
                                              {0.0f, 0.0f},
                                              {1.0f, 0.0f},
                                              {1.0f, 1.0f},
                                              {0.0f, 1.0f},
                                              // Right face
                                              {0.0f, 0.0f},
                                              {1.0f, 0.0f},
                                              {1.0f, 1.0f},
                                              {0.0f, 1.0f},
                                              // Bottom face
                                              {0.0f, 0.0f},
                                              {1.0f, 0.0f},
                                              {1.0f, 1.0f},
                                              {0.0f, 1.0f},
                                              // Top face
                                              {0.0f, 0.0f},
                                              {1.0f, 0.0f},
                                              {1.0f, 1.0f},
                                              {0.0f, 1.0f}};

        std::vector<unsigned int> unitCubeIndices = {// Back face
                                                     0, 1, 2, 2, 3, 0,
                                                     // Front face
                                                     4, 5, 6, 6, 7, 4,
                                                     // Left face
                                                     8, 9, 10, 10, 11, 8,
                                                     // Right face
                                                     12, 13, 14, 14, 15, 12,
                                                     // Bottom face
                                                     16, 17, 18, 18, 19, 16,
                                                     // Top face
                                                     20, 21, 22, 22, 23, 20};

        // Fill the temporary vertex with the pre-calculated data and push it into the mesh's vertices
        for (size_t i = 0; i < preCalcPositions.size(); ++i)
        {
            // Directly plug in the pre-calculated vertex positions, normals, and UVs
            tmpVertex.position = preCalcPositions[i];
            tmpVertex.normal = Math::Normalized(preCalcNormals[i]);
            tmpVertex.uv = preCalcUVs[i];

            // Push the vertex into the mesh's vertices
            mesh.vertices.emplace_back(tmpVertex);
        }

        mesh.indices = unitCubeIndices;
    }

    std::string Cube::GetName()
    {
        return mesh.name;
    }

    Rendering::Assets::Mesh& Cube::GetCubeMesh()
    {
        return mesh;
    }

    std::vector<unsigned int>& Cube::GetIndices()
    {
        return mesh.indices;
    }

    std::vector<Rendering::Vertex>& Cube::GetVertices()
    {
        return mesh.vertices;
    }

    Cube::~Cube()
    {
    }
} // namespace Vakol

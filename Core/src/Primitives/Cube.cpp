#include "Primitives/Cube.hpp"

namespace Vakol
{
    Cube::Cube()
    {
        GUID tmpGUID;         // Create a temporary GUID object
        tmpGUID.GenNewGUID(); // Generate a new GUID

        mesh.name = "DEFAULT_CUBE_mesh";     // Set the name to DEFAULT_CUBE_mesh
        mesh.ID = tmpGUID.ConvertToString(); // Generate a new GUID for the mesh

        GenerateData(); // Generate the data for the cube
    }

    Cube::Cube(std::string inputName, std::string inputID, Math::Vec3 halfExtents)
    {
        mesh.name = inputName + "_mesh"; // Set the name to the input name
        mesh.ID = inputID;               // Generate a new GUID for the mesh

        if (halfExtents == Math::Vec3(0.0f, 0.0f, 0.0f))
        {
            GenerateData(); // Generate the data for the cube
        }
        else
        {
            GenerateData(halfExtents); // Generate the data for the cube with the input half extents
        }
    }

    void Cube::GenerateData(Math::Vec3 halfExtents)
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
            // Check if the half extents are 0, if so, use the pre-calculated positions, otherwise, scale them by the
            // half extents
            if (halfExtents == Math::Vec3(0.0, 0.0, 0.0))
            {
                tmpVertex.position = preCalcPositions[i];
            }
            else
                tmpVertex.position = preCalcPositions[i] * (halfExtents * 2.0f);

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

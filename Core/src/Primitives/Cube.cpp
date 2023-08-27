#include "Primitives/Cube.hpp"

namespace Vakol
{
    Cube::Cube()
    {
        cubeTransform.pos = Math::Vec3(0.0f, 0.0f, 0.0f);               // Set the position to 0
        cubeTransform.scale = Math::Vec3(1.0f, 1.0f, 1.0f);             // Set the scale to 1
        cubeTransform.rot = Math::Quat(0.0f, 0.0f, 0.0f, 1.0f);         // Set the rotation to 0
        cubeTransform.eulerAngles = Math::Vec3(0.0f, 0.0f, 0.0f);       // Set the euler angles to 0

        mesh.name = "DEFAULT_CUBE_mesh";                                // Set the name to DEFAULT_CUBE_mesh
        mesh.ID = xg::newGuid().str();                                  // Generate a new GUID for the mesh

        GenerateData(cubeTransform);                                    // Generate the data for the cube 
    }

    Cube::Cube(Components::Transform inputTransform, std::string inputName, std::string inputID) 
    {
        Math::Normalized(inputTransform.rot);                           // Normalize the rotation quaternion

        cubeTransform.pos = inputTransform.pos;                         // Set the position to the input position
        cubeTransform.scale = inputTransform.scale;                     // Set the scale to the input scale
        cubeTransform.rot = inputTransform.rot;                         // Set the rotation to the input rotation
        cubeTransform.eulerAngles = inputTransform.eulerAngles;         // Set the euler angles to the input euler angles

        mesh.name = inputName + "_mesh";                                // Set the name to the input name
        mesh.ID = inputID;                                              // Generate a new GUID for the mesh

        GenerateData(inputTransform);                                   // Generate the data for the cube
    }

    void Cube::GenerateData(Components::Transform inputTransform)
    {
        Rendering::Vertex tmpVertex;                                    // A temporary vertex object to store the data

        inputTransform.rot = Math::Normalized(inputTransform.rot);      // Normalize the rotation quaternion (Prevents the rotation from accidentally scaling the vertices)

        // Get the model matrix
        Math::Mat4 modelMatrix = Rendering::RenderAPI::GetModelMatrix(inputTransform);  

        // Compute the inverse transpose of the model matrix
        Math::Mat4 inverseTranspose = Math::Transpose(Math::Inverse(modelMatrix));

        tmpVertex.bitangent = Math::Vec3(0.0f, 0.0f, 0.0f);             // Set the bitangent to 0
        tmpVertex.tangent = Math::Vec3(0.0f, 0.0f, 0.0f);               // Set the tangent to 0

        // Use pre-calculated positions for the cube
        std::vector<Math::Vec3> preCalcPositions = 
        {
            // Back face
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
            {-0.5f, 0.5f, 0.5f}
        }; 

        // Use pre-calculated normals for the cube
        std::vector<Math::Vec3> preCalcNormals = 
        {
            // Back face
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
            {0.0f, 1.0f, 0.0f}
        };

        // Use pre-calculated UVs for the cube
        std::vector<Math::Vec2> preCalcUVs = 
        {
            // Back face
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
            {0.0f, 1.0f}
        };

        std::vector<unsigned int> unitCubeIndices = 
        {
            // Back face
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
            20, 21, 22, 22, 23, 20
        };

        std::vector<Math::Vec3> preCalcTangents = 
        {
            // Back face
            {1.0f, 0.0f, 0.0f},
            // Front face
            {-1.0f, 0.0f, 0.0f},
            // Left face
            {0.0f, 0.0f, -1.0f},
            // Right face
            {0.0f, 0.0f, 1.0f},
            // Bottom face
            {1.0f, 0.0f, 0.0f},
            // Top face
            {1.0f, 0.0f, 0.0f}
        };

        std::vector<Math::Vec3> preCalcBitangents = 
        {
            // Back face
            {0.0f, -1.0f, 0.0f},
            // Front face
            {0.0f, -1.0f, 0.0f},
            // Left face
            {0.0f, -1.0f, 0.0f},
            // Right face
            {0.0f, -1.0f, 0.0f},
            // Bottom face
            {0.0f, 0.0f, -1.0f},
            // Top face
            {0.0f, 0.0f, 1.0f}
        };
        
        // Create a temporary vertex to store the pre-calculated data, apply transformations, and push back into the final vertex list
        for (size_t i = 0; i < preCalcPositions.size(); ++i)
        {
            // Directly plug in the pre-calculated vertex positions, normals, and UVs
            tmpVertex.position = preCalcPositions[i];
            tmpVertex.normal = preCalcNormals[i];
            tmpVertex.uv = preCalcUVs[i];
            tmpVertex.tangent = preCalcTangents[i];
            tmpVertex.bitangent = preCalcBitangents[i];

            // Apply transformations to the vertex
            // Apply scaling
            tmpVertex.position *= inputTransform.scale;

            // Apply rotation using quaternion
            tmpVertex.position = inputTransform.rot * tmpVertex.position;

            // Transform and normalize the normal, tangent, and bitangent
            tmpVertex.normal = Math::Vec3(inverseTranspose * Math::Vec4(tmpVertex.normal, 0.0f));
            tmpVertex.tangent = Math::Vec3(inverseTranspose * Math::Vec4(tmpVertex.tangent, 0.0f));
            tmpVertex.bitangent = Math::Vec3(inverseTranspose * Math::Vec4(tmpVertex.bitangent, 0.0f));

            tmpVertex.normal = Math::Normalized(tmpVertex.normal);
            tmpVertex.tangent = Math::Normalized(tmpVertex.tangent);
            tmpVertex.bitangent = Math::Normalized(tmpVertex.bitangent);

            // Push the vertex into the mesh's vertices
            mesh.vertices.push_back(tmpVertex);
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
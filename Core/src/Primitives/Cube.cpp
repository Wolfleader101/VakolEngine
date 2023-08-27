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
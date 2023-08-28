#include "Primitives/Sphere.hpp"

namespace Vakol
{
    Sphere::Sphere()
    {
        stacks = 10;                       // Set the default number of stacks to 10
        sectors = 10;                      // Set the default number of sectors to 10
        mesh.name = "DEFAULT_SPHERE_mesh"; // Set the default name to DEFAULT_SPHERE_mesh
        mesh.ID = xg::newGuid().str();     // Generate a new GUID for the mesh

        GenerateData(1.0f, 10, 10); // Generate the data for the sphere
    }

    Sphere::Sphere(double inputRadius, unsigned inputStacks, unsigned inputSectors, std::string inputName,
                   std::string inputID)
    {
        stacks = inputStacks;            // Set the number of stacks to the input number of stacks
        sectors = inputSectors;          // Set the number of sectors to the input number of sectors
        mesh.name = inputName + "_mesh"; // Set the name to the input name
        mesh.ID = inputID;               // Generate a new GUID for the mesh

        GenerateData(inputRadius, inputStacks, inputSectors); // Generate the data for the sphere
    }

    void Sphere::GenerateData(double inputRadius, unsigned inputStacks, unsigned inputSectors)
    {
        Rendering::Vertex tmpVertex;        // A temporary vertex object to store the data
        Components::Transform tmpTransform; // A temporary transform object to store the data

        tmpTransform.pos = Math::Vec3(0.0f, 0.0f, 0.0f);         // Set the position to 0
        tmpTransform.scale = Math::Vec3(1.0f, 1.0f, 1.0f);       // Set the scale to 1
        tmpTransform.rot = Math::Quat(1.0f, 0.0f, 0.0f, 0.0f);   // Set the rotation to 0
        tmpTransform.eulerAngles = Math::Vec3(0.0f, 0.0f, 0.0f); // Set the euler angles to 0

        tmpTransform.rot = Math::Normalized(tmpTransform.rot); // Normalize the rotation quaternion (Prevents the
                                                               // rotation from accidentally scaling the vertices)

        tmpVertex.bitangent = Math::Vec3(0.0f, 0.0f, 0.0f); // Set the bitangent to 0
        tmpVertex.tangent = Math::Vec3(0.0f, 0.0f, 0.0f);   // Set the tangent to 0

        double x, y, z, xy;                    // Vertex Position
        double lengthInv = 1.0f / inputRadius; // Vertex Normal
        double s, t;                           // Vertex UV Coordinates

        double sectorStep = 2 * Math::PiDouble() / inputSectors; // The angle between each sector
        double stackStep = Math::PiDouble() / inputStacks;       // The angle between each stack
        double sectorAngle, stackAngle;                          // The angle of the sector and stack

        unsigned k1, k2; // The indices of the vertices

        // Iterate through the stacks and generate the vertices, normals and texture coordinates
        for (int i = 0; i <= inputStacks; ++i)
        {
            stackAngle = Math::PiDouble() / 2 - i * stackStep; // Starting from pi/2 to -pi/2
            xy = inputRadius * cos(stackAngle);                // r * cos(u)
            z = (inputRadius * sin(stackAngle)) +
                tmpTransform.pos.z; // r * sin(u) (Add position to move sphere to point)

            // The first and last vertices have same position and normal, but different tex coordinates
            for (int j = 0; j <= inputSectors; ++j)
            {
                sectorAngle = j * sectorStep; // starting from 0 to 2pi

                // Vertex position (x, y, z)
                x = xy * (cos(sectorAngle)) +
                    tmpTransform.pos.x; // r * cos(u) * cos(v) (Add position to move sphere to point)
                y = xy * (sin(sectorAngle)) +
                    tmpTransform.pos.y; // r * cos(u) * sin(v) (Add position to move sphere to point)

                // Set the position of the vertex
                tmpVertex.position = Math::Vec3(x, y, z);

                // Apply scaling
                tmpVertex.position *= tmpTransform.scale;

                // Apply rotation using quaternion
                tmpVertex.position = tmpTransform.rot * tmpVertex.position;

                // Normalized vertex normal and set
                tmpVertex.normal = Math::Vec3(x * lengthInv, y * lengthInv, z * lengthInv);

                // Apply scaling to the normal
                tmpVertex.normal *= tmpTransform.scale;

                // Apply rotation to the normal
                tmpVertex.normal = tmpTransform.rot * tmpVertex.normal;

                // Vertex texture coordinate (s, t) range between [0, 1]
                s = (double)j / inputSectors; // u Coordinate
                t = (double)i / inputStacks;  // v Coordinate

                tmpVertex.uv = Math::Vec2(s, t);

                mesh.vertices.push_back(tmpVertex);
            }
        }

        // Generate the indices
        for (int i = 0; i < inputStacks; ++i)
        {
            k1 = i * (inputSectors + 1); // Beginning of current stack
            k2 = k1 + inputSectors + 1;  // Beginning of next stack

            for (int j = 0; j < inputSectors; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding first and last stacks
                // k1 => k2 => k1+1
                if (i != 0)
                {
                    mesh.indices.push_back(k1);
                    mesh.indices.push_back(k2);
                    mesh.indices.push_back(k1 + 1);
                }

                // k1+1 => k2 => k2+1
                if (i != (inputStacks - 1))
                {
                    mesh.indices.push_back(k1 + 1);
                    mesh.indices.push_back(k2);
                    mesh.indices.push_back(k2 + 1);
                }
            }
        }

        // Create the individual transformation matrices
        Math::Mat4 translationMatrix = Math::Translation(tmpTransform.pos);
        Math::Mat4 rotationMatrix = Math::Mat4Cast(tmpTransform.rot);
        Math::Mat4 scaleMatrix = Math::Scale(tmpTransform.scale);

        // Combine them to create the full transformation matrix
        Math::Mat4 transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;

        // Calculate the inverse transpose
        Math::Mat4 inverseTranspose = Math::Transpose(Math::Inverse(transformationMatrix));

        // Transform the normals
        for (Rendering::Vertex& vertex : mesh.vertices)
        {
            // Extract the 3x3 part of the inverse transpose matrix
            Math::Mat3 inverseTranspose3x3 = Math::Mat3(inverseTranspose);

            // Multiply the normal by the 3x3 matrix
            vertex.normal = inverseTranspose3x3 * vertex.normal;

            // Normalize the normal
            Math::Normalize(vertex.normal);
        }

        originalVertices = mesh.vertices; // Store the original vertices
    }

    std::string Sphere::GetName()
    {
        return mesh.name;
    }

    Rendering::Assets::Mesh& Sphere::GetSphereMesh()
    {
        return mesh;
    }

    std::vector<unsigned int>& Sphere::GetIndices()
    {
        return mesh.indices;
    }

    std::vector<Rendering::Vertex>& Sphere::GetVertices()
    {
        return mesh.vertices;
    }

    Sphere::~Sphere()
    {
    }
} // namespace Vakol

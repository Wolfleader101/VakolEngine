#include "Primitives/Sphere.hpp"

namespace Vakol
{
    Sphere::Sphere()
    {
        stacks = 10;                       // Set the default number of stacks to 10
        sectors = 10;                      // Set the default number of sectors to 10
        mesh.name = "DEFAULT_SPHERE_mesh"; // Set the default name to DEFAULT_SPHERE_mesh
        mesh.ID = xg::newGuid().str();     // Generate a new GUID for the mesh

        GenerateData(1.0f, stacks, sectors); // Generate the data for the sphere with a radius of 1
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
        Rendering::Vertex tmpVertex; // A temporary vertex object to store the data

        double x, y, z, xy; // Vertex Position
        double s, t;        // Vertex UV Coordinates

        double sectorStep = 2 * Math::PiDouble() / inputSectors; // The angle between each sector
        double stackStep = Math::PiDouble() / inputStacks;       // The angle between each stack
        double sectorAngle, stackAngle;                          // The angle of the sector and stack

        unsigned k1, k2; // The indices of the vertices

        // Iterate through the stacks and generate the vertices, normals, and texture coordinates
        for (int i = 0; i <= inputStacks; ++i)
        {
            stackAngle = Math::PiDouble() / 2 - i * stackStep; // Starting from pi/2 to -pi/2
            xy = inputRadius * cos(stackAngle);                // r * cos(u)
            z = inputRadius * sin(stackAngle);                 // r * sin(u)

            for (int j = 0; j <= inputSectors; ++j)
            {
                sectorAngle = j * sectorStep; // starting from 0 to 2pi

                // Vertex position (x, y, z)
                x = xy * cos(sectorAngle); // r * cos(u) * cos(v)
                y = xy * sin(sectorAngle); // r * cos(u) * sin(v)

                // Set the position and normal of the vertex
                tmpVertex.position = Math::Vec3(x, y, z);
                tmpVertex.normal = Math::Normalized(Math::Vec3(x, y, z)); // Directly normalize the vertex position

                // Vertex texture coordinate (s, t) range between [0, 1]
                s = (double)j / inputSectors; // u Coordinate
                t = (double)i / inputStacks;  // v Coordinate

                tmpVertex.uv = Math::Vec2(s, t);

                mesh.vertices.emplace_back(tmpVertex);
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
                    mesh.indices.emplace_back(k1);
                    mesh.indices.emplace_back(k2);
                    mesh.indices.emplace_back(k1 + 1);
                }

                // k1+1 => k2 => k2+1
                if (i != (inputStacks - 1))
                {
                    mesh.indices.emplace_back(k1 + 1);
                    mesh.indices.emplace_back(k2);
                    mesh.indices.emplace_back(k2 + 1);
                }
            }
        }
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

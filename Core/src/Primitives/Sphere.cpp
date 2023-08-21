#include "Primitives/Sphere.hpp"

namespace Vakol
{
    Sphere::Sphere()
    {
        position = Math::Vec3(0.0f, 0.0f, 0.0f);
        scale = Math::Vec3(1.0f, 1.0f, 1.0f);
        rotation = Math::Quat(0.0f, 0.0f, 0.0f, 1.0f);

        stacks = 0;
        sectors = 0;
	}

    Sphere::Sphere(Math::Vec3 inputPosition, double inputRadius, unsigned inputStacks, unsigned inputSectors)
    {
        position = inputPosition; 
        scale = Math::Vec3(1.0f, 1.0f, 1.0f);
        rotation = Math::Quat(0.0f, 0.0f, 0.0f, 1.0f);

        stacks = inputStacks;
        sectors = inputSectors; 

        GenerateData(inputPosition, inputRadius, inputStacks, inputSectors);
    }

    void Sphere::GenerateData(Math::Vec3 inputPosition, double inputRadius, unsigned inputStacks, unsigned inputSectors)
    {
        Rendering::Vertex tmpVertex;                                        // A temporary vertex object to store the data

        tmpVertex.bitangent = Math::Vec3(0.0f, 0.0f, 0.0f);                 // Set the bitangent to 0
        tmpVertex.tangent = Math::Vec3(0.0f, 0.0f, 0.0f);                   // Set the tangent to 0

        double x, y, z, xy;                                                 // Vertex Position
        double nx, ny, nz, lengthInv = 1.0f / inputRadius;                  // Vertex Normal
        double s, t;                                                        // Vertex UV Coordinates

        double sectorStep = 2 * M_PI / inputSectors;                        // The angle between each sector
        double stackStep = M_PI / inputStacks;                              // The angle between each stack
        double sectorAngle, stackAngle;                                     // The angle of the sector and stack

        unsigned k1, k2;                                                    // The indices of the vertices

        // Iterate through the stacks and generate the vertices, normals and texture coordinates
        for (int i = 0; i <= inputStacks; ++i)
        {
            stackAngle = M_PI / 2 - i * stackStep;                          // Starting from pi/2 to -pi/2
            xy = inputRadius * cos(stackAngle);                             // r * cos(u)
            z = (inputRadius * sin(stackAngle)) + inputPosition.z;          // r * sin(u) (Add position to move sphere to point)

            // The first and last vertices have same position and normal, but different tex coordinates
            for (int j = 0; j <= inputSectors; ++j)
            {
                sectorAngle = j * sectorStep; // starting from 0 to 2pi

                // Vertex position (x, y, z)
                x = xy * (cos(sectorAngle)) + inputPosition.x;              // r * cos(u) * cos(v) (Add position to move sphere to point)
                y = xy * (sin(sectorAngle)) + inputPosition.y;              // r * cos(u) * sin(v) (Add position to move sphere to point) 

                tmpVertex.position = Math::Vec3(x, y, z);

                // Normalized vertex normal (nx, ny, nz)
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;

                tmpVertex.normal = Math::Vec3(nx, ny, nz);

                // Vertex texture coordinate (s, t) range between [0, 1]
                s = (double)j / inputSectors;                               // u Coordinate
                t = (double)i / inputStacks;                                // v Coordinate

                tmpVertex.uv = Math::Vec2(s, t);

                mesh.vertices.push_back(tmpVertex);
            }
        }

        // Generate the indices
        for (int i = 0; i < inputStacks; ++i)
        {
            k1 = i * (inputSectors + 1); // beginning of current stack
            k2 = k1 + inputSectors + 1;  // beginning of next stack

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
    }

    Sphere::~Sphere()
    {

	}
} // namespace Vakol
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

    void Sphere::CreateSphere(Math::Vec3 position, double radius, unsigned stacks, unsigned sectors)
    {
        Rendering::Vertex tmpVertex; // A temporary vertex object to store the data

        double x, y, z, xy;                           // Vertex Position
        double nx, ny, nz, lengthInv = 1.0f / radius; // Vertex Normal
        double s, t;                                  // Vertex UV Coordinates

        double sectorStep = 2 * M_PI / sectors; // The angle between each sector
        double stackStep = M_PI / stacks;       // The angle between each stack
        double sectorAngle, stackAngle;         // The angle of the sector and stack

        unsigned k1, k2; // The indices of the vertices

        // Iterate through the stacks and generate the vertices, normals and texture coordinates
        for (int i = 0; i <= stacks; ++i)
        {
            stackAngle = M_PI / 2 - i * stackStep;       // Starting from pi/2 to -pi/2
            xy = radius * cos(stackAngle);               // r * cos(u)
            z = (radius * sin(stackAngle)) + position.z; // r * sin(u) (Add position to move sphere to point)

            // The first and last vertices have same position and normal, but different tex coordinates
            for (int j = 0; j <= sectors; ++j)
            {
                sectorAngle = j * sectorStep; // starting from 0 to 2pi

                // Vertex position (x, y, z)
                x = xy * (cos(sectorAngle)) + position.x; // r * cos(u) * cos(v) (Add position to move sphere to point)
                y = xy * (sin(sectorAngle)) + position.y; // r * cos(u) * sin(v) (Add position to move sphere to point)

                tmpVertex.position = Math::Vec3(x, y, z);

                // Normalized vertex normal (nx, ny, nz)
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;

                tmpVertex.normal = Math::Vec3(nx, ny, nz);

                // Vertex texture coordinate (s, t) range between [0, 1]
                s = (double)j / sectors; // u Coordinate
                t = (double)i / stacks;  // v Coordinate

                tmpVertex.uv = Math::Vec2(s, t);

                mesh.vertices.push_back(tmpVertex);
            }
        }

        // Generate the indices
        for (int i = 0; i < stacks; ++i)
        {
            k1 = i * (sectors + 1); // beginning of current stack
            k2 = k1 + sectors + 1;  // beginning of next stack

            for (int j = 0; j < sectors; ++j, ++k1, ++k2)
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
                if (i != (stacks - 1))
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
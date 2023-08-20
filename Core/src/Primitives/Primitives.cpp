#include "Primitives/Primitives.hpp"

namespace Vakol
{
    Primitives::Primitives()
    {

    }

    void Primitives::CreateSphere(Math::Vec3 position, Math::Vec3 scale, Math::Quat rotation, unsigned stacks, unsigned sectors)
    {
        Sphere tmpSphere;
        Vertex tmpVertex;

        double sectorStep = 2 * M_PI / sectors;
        double stackStep = M_PI / stacks; 
        double sectorAngle, stackAngle;
        
        for (unsigned i = 0; i <= stacks; ++i)
        {
			stackAngle = M_PI / 2 - i * stackStep;
			double xy = cos(stackAngle); 
			double z = sin(stackAngle); 

            for (unsigned j = 0; j <= sectors; ++j)
            {
				sectorAngle = j * sectorStep;

				double x = xy * cos(sectorAngle);
				double y = xy * sin(sectorAngle);

				tmpSphere.mesh.vertices.push_back(Math::Vec3(x, y, z));
			}
		}
	}

    void Primitives::CreateCube(Math::Vec3 position, Math::Vec3 scale, Math::Quat rotation)
    {
        Cube tmpCube;
    }

    Primitives::~Primitives()
    {
        
    }
} // namespace Vakol
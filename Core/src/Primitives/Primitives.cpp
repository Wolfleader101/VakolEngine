#include "Primitives/Primitives.hpp"

namespace Vakol
{
    Primitives::Primitives()
    {
        
    }

    void Primitives::CreateSphere(Math::Vec3 inputPosition, double inputRadius, unsigned inputStacks, unsigned inputSectors)
    {
        Sphere tmpSphere;                                                               // Create a new Sphere object
        tmpSphere.GenerateData(inputPosition, inputRadius, inputStacks, inputSectors);  // Generate the data for the sphere
        m_Spheres.push_back(tmpSphere);                                                 // Add the sphere to the vector
    }

    void Primitives::CreateCube(Math::Vec3 position, Math::Vec3 scale, Math::Quat rotation)
    {
        Cube tmpCube;
    }

    Primitives::~Primitives()
    {
        
    }
} // namespace Vakol
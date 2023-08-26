#include "Primitives/Primitives.hpp"

namespace Vakol
{
    Primitives::Primitives()
    {
        
    }

    void Primitives::CreateSphere(Components::Transform& inputTransform, double inputRadius, unsigned inputStacks, unsigned inputSectors, std::string inputName)
    {
        Rendering::VertexArray tmpVertexArray; 												    // Create a temporary vertex array
        
        xg::Guid sphereGUID = xg::newGuid();                                                    // Generate a new GUID for the sphere
        Sphere tmpSphere(inputTransform, inputRadius, inputStacks, inputSectors, inputName);    // Create a new Sphere object with input data

        m_Spheres[sphereGUID] = tmpSphere;                                                      // Add the sphere to the storage map
        nameToGuidMap[inputName] = sphereGUID;                                                  // Add the name-GUID mapping

        tmpVertexArray.ID = sphereGUID;                                                         // Set the ID of the vertex array to the GUID of the sphere 
        tmpVertexArray.indices = tmpSphere.GetIndices();                                        // Get the indices of the sphere
        tmpVertexArray.vertices = tmpSphere.GetVertices();                                      // Get the vertices of the sphere

        VK_INFO("Sphere '" + inputName + "' with GUID '" + sphereGUID.str() + "' created!");
    }

    xg::Guid Primitives::GetGuidByName(const std::string& name)
    {
        return nameToGuidMap[name];
    }

    Primitives::~Primitives()
    {
        
    }
} // namespace Vakol
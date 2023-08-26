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

    Rendering::Assets::Model Primitives::GetShape(ShapeType type, xg::Guid inputGUID)
    {
        // Switch between the different types of shapes
        switch (type)
        {
            case SPHERE: 
            {
                // Look up the sphere in the map using the GUID
                auto it = m_Spheres.find(inputGUID);

                // Check if the sphere was found
                if (it != m_Spheres.end())
                {
                    // Create and populate the VertexArray for this object
                    Rendering::VertexArray tmpVertexArray;

                    tmpVertexArray.ID = inputGUID;

                    // Assuming the sphere is represented by the first mesh in the model
                    if (!it->second.meshes.empty())
                    {
                        tmpVertexArray.indices = it->second.meshes[0].indices;
                        tmpVertexArray.vertices = it->second.meshes[0].vertices;
                    }
                    else
                    {
                        VK_ERROR("Sphere model does not contain any meshes! Returning an empty model...");

                        return Rendering::Assets::Model{}; // Return an empty model if the sphere does not contain any meshes
                    }

                    VK_INFO("Rendering Sphere");

                    // Generate the vertex commands for the object and render it
                    Rendering::RenderAPI::GenerateVertexCommand(std::move(tmpVertexArray));

                    return it->second; // Return the found sphere
                }
                else
                {
                    VK_ERROR("Sphere with GUID '" + inputGUID.str() + "' not found!");
                }

                break;
            }
            default:
                VK_ERROR("The primitive shape type is not valid!");
                break;
        }
    }

    xg::Guid Primitives::GetGuidByName(const std::string& name)
    {
        return nameToGuidMap[name];
    }

    Primitives::~Primitives()
    {
        
    }
} // namespace Vakol
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

        int highestCount = -1;

        for (const auto& pair : nameToGuidMap)
        {
            std::string existingName = pair.first; 											    // Get the name of the existing sphere

            if (existingName.find(inputName) == 0)                                              // Check if the existing name starts with the input name
            {
                std::string suffix = existingName.substr(inputName.length()); 				    // Get the suffix of the existing name

                // Check if the suffix is empty or if it is a number
                if (suffix.empty() || (suffix[0] == '_' && std::all_of(suffix.begin() + 1, suffix.end(), ::isdigit)))
                {
                    int count = suffix.empty() ? 0 : std::stoi(suffix.substr(1)); 			    // Get the count of the existing name
                    highestCount = std::max(highestCount, count); 							    // Update the highest count
                }
            }
        }

        std::string uniqueName = inputName + (highestCount >= 0 ? "_" + std::to_string(highestCount + 1) : "");

        Sphere tmpSphere(inputTransform, inputRadius, inputStacks, inputSectors, uniqueName);   // Create a new Sphere object with input data

        m_Spheres[sphereGUID] = GetShape(SPHERE, sphereGUID);                                   // Add the sphere to the storage map 
        nameToGuidMap[uniqueName] = sphereGUID;                                                 // Add the name-GUID mapping

        m_Spheres[sphereGUID].name = uniqueName;                                                // Set the name of the sphere
        m_Spheres[sphereGUID].path = uniqueName + "_GENERATED";                                 // Set a dummy path for the sphere

        tmpVertexArray.ID = sphereGUID;                                                         // Set the ID of the vertex array to the GUID of the sphere 
        tmpVertexArray.indices = tmpSphere.GetIndices();                                        // Get the indices of the sphere
        tmpVertexArray.vertices = tmpSphere.GetVertices();                                      // Get the vertices of the sphere

        Rendering::RenderAPI::GenerateVertexCommand(std::move(tmpVertexArray));                 // Create the vertex array in OpenGL

        AssetLoader::AddModel(GetShape(SPHERE, sphereGUID));                                    // Add the sphere to the AssetLoader

        VK_INFO("Sphere '" + uniqueName + "' with GUID '" + sphereGUID.str() + "' created!"); 
    }

    Rendering::Assets::Model& Primitives::GetShape(ShapeType type, xg::Guid inputGUID)
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
                    // Assuming the sphere is represented by the first mesh in the model
                    if (!it->second.meshes.empty())
                    {
                        return it->second; // Return the found sphere
                    }
                    else
                    {
                        VK_ERROR("Sphere with GUID '" + inputGUID.str() + "' has no meshes! Exiting...");   // Log an error if the sphere does not contain any meshes
                        std::exit(EXIT_FAILURE);                                                            // Terminate the program
                    }
                }
                else
                {
                    VK_ERROR("Sphere with GUID '" + inputGUID.str() + "' not found! Exiting...");           // Log an error if the sphere was not found
                    std::exit(EXIT_FAILURE);                                                                // Terminate the program
                }

                break;
            }
            default:
                VK_ERROR("The primitive shape type is not valid! Exiting...");  // Log an error if the shape type is not valid
                std::exit(EXIT_FAILURE);                                        // Terminate the program
        }
    }

    xg::Guid Primitives::GetGuidByName(const std::string& name)
    {
        if (nameToGuidMap.find(name) != nameToGuidMap.end())  
        {
            return nameToGuidMap[name];
        }
    }

    Primitives::~Primitives()
    {
        
    }
} // namespace Vakol
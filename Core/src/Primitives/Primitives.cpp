#include "Primitives/Primitives.hpp"

namespace Vakol
{
    Primitives::Primitives()
    {
        
    }

    void Primitives::CreateSphere(Components::Transform& inputTransform, double inputRadius, unsigned inputStacks, unsigned inputSectors, std::string inputName)
    {
        Rendering::VertexArray tmpVertexArray; 												    // Create a temporary vertex array
        Rendering::Assets::Model tmpModel; 													    // Create a temporary model
        Rendering::Assets::Material tmpMaterial; 												// Create a temporary material
        Rendering::Assets::Mesh tmpMesh;                                                        // Create a temporary mesh
        
        xg::Guid sphereGUID = xg::newGuid();                                                    // Generate a new GUID for the sphere

        int highestCount = -1;

        for (const auto& pair : nameToGuidMap)
        {
            std::string existingName = pair.first; 											                    // Get the name of the existing sphere

            if (existingName.find(inputName) == 0)                                                              // Check if the existing name starts with the input name
            {
                std::string suffix = existingName.substr(inputName.length()); 				                    // Get the suffix of the existing name

                // Check if the suffix is empty or if it is a number
                if (suffix.empty() || (suffix[0] == '_' && std::all_of(suffix.begin() + 1, suffix.end(), ::isdigit)))
                {
                    int count = suffix.empty() ? 0 : std::stoi(suffix.substr(1)); 			                    // Get the count of the existing name
                    highestCount = std::max(highestCount, count); 							                    // Update the highest count
                }
            }
        }

        std::string uniqueName = inputName + (highestCount >= 0 ? "_" + std::to_string(highestCount + 1) : "");

        Sphere tmpSphere(inputTransform, inputRadius, inputStacks, inputSectors, uniqueName, sphereGUID.str()); // Create a new Sphere object with input data

        tmpModel.name = uniqueName;                                                                             // Set the name of the model
        tmpMesh = tmpSphere.GetSphereMesh();                                                                    // Get the mesh of the sphere
        tmpMesh.material = std::make_shared<Vakol::Rendering::Assets::Material>(tmpMaterial);                   // Set the material of the mesh  
        tmpMesh.material->name = uniqueName + "_shader";                                                        // Set the name of the material

        tmpModel.meshes.push_back(tmpMesh);                                                                     // Add the mesh of the sphere to the model

        tmpModel.path = uniqueName + "_GENERATED";                                                              // Set a dummy path for the model

        m_Spheres[sphereGUID] = tmpModel;                                                                       // Add the model to the map of spheres

        nameToGuidMap[uniqueName] = sphereGUID;                                                                 // Add the name-GUID mapping

        tmpVertexArray.ID = sphereGUID;                                                                         // Set the ID of the vertex array to the GUID of the sphere
        tmpVertexArray.indices = tmpSphere.GetIndices();                                                        // Get the indices of the sphere
        tmpVertexArray.vertices = tmpSphere.GetVertices();                                                      // Get the vertices of the sphere

        Rendering::RenderAPI::GenerateVertexCommand(std::move(tmpVertexArray));                                 // Create the vertex array in OpenGL

        VK_INFO("Sphere '" + uniqueName + "' with GUID '" + sphereGUID.str() + "' created!");
    }

    void Primitives::CreateCube(Components::Transform& inputTransform, std::string inputName)
    {
        Rendering::VertexArray tmpVertexArray;   // Create a temporary vertex array
        Rendering::Assets::Model tmpModel;       // Create a temporary model
        Rendering::Assets::Material tmpMaterial; // Create a temporary material
        Rendering::Assets::Mesh tmpMesh;         // Create a temporary mesh

        xg::Guid cubeGUID = xg::newGuid(); // Generate a new GUID for the cube

        int highestCount = -1;

        for (const auto& pair : nameToGuidMap)
        {
            std::string existingName = pair.first; // Get the name of the existing cube

            if (existingName.find(inputName) == 0) // Check if the existing name starts with the input name
            {
                std::string suffix = existingName.substr(inputName.length()); // Get the suffix of the existing name

                // Check if the suffix is empty or if it is a number
                if (suffix.empty() || (suffix[0] == '_' && std::all_of(suffix.begin() + 1, suffix.end(), ::isdigit)))
                {
                    int count = suffix.empty() ? 0 : std::stoi(suffix.substr(1)); // Get the count of the existing name
                    highestCount = std::max(highestCount, count);                 // Update the highest count
                }
            }
        }

        std::string uniqueName = inputName + (highestCount >= 0 ? "_" + std::to_string(highestCount + 1) : "");

        Cube tmpCube(inputTransform, uniqueName, cubeGUID.str()); // Create a new Cube object with input data

        tmpModel.name = uniqueName;      // Set the name of the model
        tmpMesh = tmpCube.GetCubeMesh(); // Get the mesh of the cube
        tmpMesh.material = std::make_shared<Vakol::Rendering::Assets::Material>(tmpMaterial); // Set the material of the mesh
        tmpMesh.material->name = uniqueName + "_shader";                       // Set the name of the material

        tmpModel.meshes.push_back(tmpMesh); // Add the mesh of the cube to the model

        tmpModel.path = uniqueName + "_GENERATED"; // Set a dummy path for the model

        m_Cubes[cubeGUID] = tmpModel; // Add the model to the map of cubes

        nameToGuidMap[uniqueName] = cubeGUID; // Add the name-GUID mapping

        tmpVertexArray.ID = cubeGUID;                    // Set the ID of the vertex array to the GUID of the cube
        tmpVertexArray.indices = tmpCube.GetIndices();   // Get the indices of the cube
        tmpVertexArray.vertices = tmpCube.GetVertices(); // Get the vertices of the cube

        Rendering::RenderAPI::GenerateVertexCommand(std::move(tmpVertexArray)); // Create the vertex array in OpenGL

        VK_INFO("Cube '" + uniqueName + "' with GUID '" + cubeGUID.str() + "' created!");
    }

    Rendering::Assets::Model& Primitives::GetModel(ShapeType type, xg::Guid inputGUID)
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
                        VK_ERROR("Sphere with GUID '" + inputGUID.str() + "' has no meshes! Exiting...");
                        std::exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    VK_ERROR("Sphere with GUID '" + inputGUID.str() + "' not found! Exiting...");
                    std::exit(EXIT_FAILURE);
                }

                break;
            }
            case CUBE: 
            { 
                // Look up the cube in the map using the GUID
                auto it = m_Cubes.find(inputGUID);

                // Check if the cube was found
                if (it != m_Cubes.end())
                {
                    // Assuming the cube is represented by the first mesh in the model
                    if (!it->second.meshes.empty())
                    {
                        return it->second; // Return the found cube
                    }
                    else
                    {
                        VK_ERROR("Cube with GUID '" + inputGUID.str() + "' has no meshes! Exiting...");
                        std::exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    VK_ERROR("Cube with GUID '" + inputGUID.str() + "' not found! Exiting...");
                    std::exit(EXIT_FAILURE);
                }

                break;
            }
            default:
                VK_ERROR("The primitive shape type is not valid! Exiting...");
                std::exit(EXIT_FAILURE);
        }
    }

    xg::Guid Primitives::GetGuidByName(const std::string& name)
    {
        if (nameToGuidMap.find(name) != nameToGuidMap.end())  
        {
            return nameToGuidMap[name];
        }
        else
        {
			VK_ERROR("Primitive with name '" + name + "' not found! Exiting...");
			std::exit(EXIT_FAILURE); 
		}
    }

    Primitives::~Primitives()
    {
        
    }
} // namespace Vakol
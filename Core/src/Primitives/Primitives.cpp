#include "Primitives/Primitives.hpp"

namespace Vakol
{
    Primitives::Primitives()
    {
    }

    void Primitives::CreateSphere(double inputRadius, unsigned inputStacks, unsigned inputSectors,
                                  std::string inputName)
    {
        GUID tmpGUID;         // Create a temporary GUID object
        tmpGUID.GenNewGUID(); // Generate a new GUID

        Rendering::VertexArray tmpVertexArray;   // Create a temporary vertex array
        Rendering::Assets::Model tmpModel;       // Create a temporary model
        Rendering::Assets::Material tmpMaterial; // Create a temporary material
        Rendering::Assets::Mesh tmpMesh;         // Create a temporary mesh

        int highestCount = -1;

        for (const auto& pair : nameToGuidMap)
        {
            std::string existingName = pair.first; // Get the name of the existing sphere

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

        Sphere tmpSphere(inputRadius, inputStacks, inputSectors, uniqueName,
                         tmpGUID.ConvertToString()); // Create a new Sphere object with input data

        tmpModel.name = uniqueName;          // Set the name of the model
        tmpMesh = tmpSphere.GetSphereMesh(); // Get the mesh of the sphere
        tmpMesh.material =
            std::make_shared<Vakol::Rendering::Assets::Material>(tmpMaterial); // Set the material of the mesh
        tmpMesh.material->name = uniqueName + "_shader";                       // Set the name of the material

        tmpModel.meshes.push_back(tmpMesh); // Add the mesh of the sphere to the model

        tmpModel.path = uniqueName + "_GENERATED"; // Set a dummy path for the model

        m_Spheres[tmpGUID.GetGUID()] = tmpModel; // Add the model to the map of spheres

        nameToGuidMap[uniqueName] = tmpGUID.GetGUID(); // Add the name-GUID mapping

        tmpVertexArray.ID = tmpGUID.GetGUID();             // Set the ID of the vertex array to the GUID of the sphere
        tmpVertexArray.indices = tmpSphere.GetIndices();   // Get the indices of the sphere
        tmpVertexArray.vertices = tmpSphere.GetVertices(); // Get the vertices of the sphere

        Rendering::RenderAPI::GenerateVertexCommand(std::move(tmpVertexArray)); // Create the vertex array in OpenGL

        VK_INFO("Sphere '" + uniqueName + "' with GUID '" + tmpGUID.ConvertToString() + "' created!");
    }

    void Primitives::CreateCube(std::string inputName, Math::Vec3 halfExtents)
    {
        Cube* tmpCube = nullptr; // Declare a pointer to Cube
        GUID tmpGUID;            // Create a temporary GUID object
        tmpGUID.GenNewGUID();    // Generate a new GUID

        Rendering::VertexArray tmpVertexArray;   // Create a temporary vertex array
        Rendering::Assets::Model tmpModel;       // Create a temporary model
        Rendering::Assets::Material tmpMaterial; // Create a temporary material
        Rendering::Assets::Mesh tmpMesh;         // Create a temporary mesh

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

        if (halfExtents == Math::Vec3(0.0, 0.0, 0.0))
        {
            tmpCube = new Cube(uniqueName, tmpGUID.ConvertToString()); // Allocate a new Cube object
        }
        else
        {
            tmpCube = new Cube(uniqueName, tmpGUID.ConvertToString(),
                               halfExtents); // Allocate a new Cube object with halfExtents
        }

        tmpModel.name = uniqueName;       // Set the name of the model
        tmpMesh = tmpCube->GetCubeMesh(); // Get the mesh of the cube
        tmpMesh.material =
            std::make_shared<Vakol::Rendering::Assets::Material>(tmpMaterial); // Set the material of the mesh
        tmpMesh.material->name = uniqueName + "_shader";                       // Set the name of the material

        tmpModel.meshes.push_back(tmpMesh); // Add the mesh of the cube to the model

        tmpModel.path = uniqueName + "_GENERATED"; // Set a dummy path for the model

        m_Cubes[tmpGUID.GetGUID()] = tmpModel; // Add the model to the map of cubes

        nameToGuidMap[uniqueName] = tmpGUID.GetGUID(); // Add the name-GUID mapping

        tmpVertexArray.ID = tmpGUID.GetGUID();            // Set the ID of the vertex array to the GUID of the cube
        tmpVertexArray.indices = tmpCube->GetIndices();   // Get the indices of the cube
        tmpVertexArray.vertices = tmpCube->GetVertices(); // Get the vertices of the cube

        Rendering::RenderAPI::GenerateVertexCommand(std::move(tmpVertexArray)); // Create the vertex array in OpenGL

        VK_INFO("Cube '" + uniqueName + "' with GUID '" + tmpGUID.ConvertToString() + "' created!");
    }

    bool Primitives::GetModel(ShapeType type, std::string inputName, Rendering::Assets::Model& outModel)
    {
        xg::Guid searchGUID = nameToGuidMap[inputName]; // Get the GUID of the model

        switch (type)
        {
        case SPHERE: {
            auto it = m_Spheres.find(searchGUID);
            if (it != m_Spheres.end())
            {
                if (!it->second.meshes.empty())
                {
                    outModel = it->second;
                    return true;
                }
                else
                {
                    VK_ERROR("Sphere with GUID '" + searchGUID.str() + "' has no meshes!");
                    return false;
                }
            }
            else
            {
                VK_ERROR("Sphere with GUID '" + searchGUID.str() + "' not found!");
                return false;
            }
            break;
        }
        case CUBE: {
            auto it = m_Cubes.find(searchGUID);
            if (it != m_Cubes.end())
            {
                if (!it->second.meshes.empty())
                {
                    outModel = it->second;
                    return true;
                }
                else
                {
                    VK_ERROR("Cube with GUID '" + searchGUID.str() + "' has no meshes!");
                    return false;
                }
            }
            else
            {
                VK_ERROR("Cube with GUID '" + searchGUID.str() + "' not found!");
                return false;
            }
            break;
        }
        default:
            VK_ERROR("The primitive shape type is not valid!");
            return false;
        }
    }

    Primitives::~Primitives()
    {
    }
} // namespace Vakol

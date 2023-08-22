#include "Primitives/Primitives.hpp"

namespace Vakol
{
    Primitives::Primitives()
    {
        
    }

    void Primitives::CreateSphere(Math::Vec3& inputPosition, double inputRadius, unsigned inputStacks, unsigned inputSectors, std::string inputName)
    {
        xg::Guid sphereGUID = xg::newGuid();                                                    // Generate a new GUID for the sphere
        Sphere tmpSphere(inputPosition, inputRadius, inputStacks, inputSectors, inputName);     // Create a new Sphere object with input data

        m_Spheres[sphereGUID] = tmpSphere;                                                      // Add the sphere to the storage map
        nameToGuidMap[inputName] = sphereGUID;                                                  // Add the name-GUID mapping
    }

    void Primitives::Scale(ShapeType type, Math::Vec3& inputScale, xg::Guid inputGUID)
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
                    // Scale the sphere
                    it->second.SetScale(inputScale);
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

    void Primitives::Position(ShapeType type, Math::Vec3& inputPosition, xg::Guid inputGUID)
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
                    // Position the sphere
                    it->second.SetPosition(inputPosition);
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

    void Primitives::Rotation(ShapeType type, Math::Quat& inputRotation, xg::Guid inputGUID)
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
                    // Rotate the sphere
                    it->second.SetRotation(inputRotation);
                }
                else
                {
                    VK_ERROR("Sphere with GUID '" + inputGUID.str() + "' not found!");
                }
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
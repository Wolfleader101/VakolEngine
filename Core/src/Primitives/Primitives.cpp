#include "Primitives/Primitives.hpp"

namespace Vakol
{
    Primitives::Primitives()
    {
        
    }

    void Primitives::CreateSphere(Math::Vec3& inputPosition, double inputRadius, unsigned inputStacks, unsigned inputSectors, std::string inputName)
    {
        Sphere tmpSphere(inputPosition, inputRadius, inputStacks, inputSectors, inputName);     // Create a new Sphere object with input data 

        m_Spheres[inputName] = tmpSphere;													    // Add the sphere to the storage map
    }

    void Primitives::Scale(ShapeType type, Math::Vec3& inputScale, std::string inputName)
    {
        // Switch between the different types of shapes
        switch (type)
        {
			case SPHERE:
            {
                // Look up the sphere in the map
                Sphere& sphere = m_Spheres[inputName];

                // Rotate the sphere
                sphere.SetScale(inputScale);

                break;
            }
			default:
                VK_ERROR("The primitive shape type is not valid!");

				break;
		}
    }

    void Primitives::Position(ShapeType type, Math::Vec3& inputPosition, std::string inputName)
    {
        // Switch between the different types of shapes
        switch (type)
        {
            case SPHERE:
            {
                // Look up the sphere in the map
                Sphere& sphere = m_Spheres[inputName];

                // Rotate the sphere
                sphere.SetPosition(inputPosition);

                break;
            }
            default:
                VK_ERROR("The primitive shape type is not valid!");

                break;
        }
    }

    void Primitives::Rotation(ShapeType type, Math::Quat& inputRotation, std::string inputName)
    {
        // Switch between the different types of shapes
        switch (type)
        {
            case SPHERE:
            {
				// Look up the sphere in the map
				Sphere& sphere = m_Spheres[inputName]; 

				// Rotate the sphere
				sphere.SetRotation(inputRotation); 

				break;
			}
            default:
                VK_ERROR("The primitive shape type is not valid!");

                break;
        }
    }

    Primitives::~Primitives()
    {
        
    }
} // namespace Vakol
#include "Primitives/Primitives.hpp"

namespace Vakol
{
    Primitives::Primitives()
    {
        
    }

    void Primitives::CreateSphere(Math::Vec3& inputPosition, double inputRadius, unsigned inputStacks, unsigned inputSectors, std::string inputName)
    {
        Sphere tmpSphere(inputPosition, inputRadius, inputStacks, inputSectors, inputName);     // Create a new Sphere object with input data
        m_Spheres.push_back(tmpSphere);                                                         // Add the sphere to the storage vector
    }

    void Primitives::Scale(ShapeType type, Math::Vec3& inputScale, std::string inputName)
    {
        // Switch between the different types of shapes
        switch (type)
        {
			case SPHERE:
                // Loop through all the spheres
                for (unsigned i = 0; i < m_Spheres.size(); i++)
                {
                    // Check if the name of the current sphere is the same as the input name
                    if (m_Spheres[i].GetName() == inputName)
                    {
						m_Spheres[i].SetScale(inputScale);	                                    // Scale the sphere
					}
				}
				break;
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
                // Loop through all the spheres
                for (unsigned i = 0; i < m_Spheres.size(); i++)
                {
                    // Check if the name of the current sphere is the same as the input name
                    if (m_Spheres[i].GetName() == inputName)
                    {
                        m_Spheres[i].SetPosition(inputPosition); // Move the sphere to a new position
                    }
                }
                break;
            default:
                VK_ERROR("The primitive shape type is not valid!");

                break;
        }
    }

    Primitives::~Primitives()
    {
        
    }
} // namespace Vakol
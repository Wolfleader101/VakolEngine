#include "ScenePhysics.hpp"
#include "ScenePhysics.hpp"
#include <Controller/Physics/ScenePhysics.hpp>
#include <Controller/Physics/PhysicsPool.hpp>

#include <Controller/System.hpp>


namespace Vakol::Controller::Physics 
{

    ScenePhysics::ScenePhysics(rp3d::PhysicsWorld* newWorld) : m_World(newWorld){};

    ScenePhysics::~ScenePhysics() { };

    void ScenePhysics::Init(){};

    void ScenePhysics::Update(const Time& time)
    {
        // Add the time difference in the accumulator
        m_accumulator += time.deltaTime;

        // While there is enough accumulated time to take
        // one or several physics steps
        while (m_accumulator >= m_timestep) {
            // Update the physics world with a constant time step
            m_World->update(m_timestep);

            // Decrease the accumulated time
            m_accumulator -= m_timestep;
        }


        // Compute the time interpolation factor
        float factor = m_accumulator / m_timestep;

        // call update on transforms 
        Vakol::Controller::System::Physics_UpdateTransforms(factor);
    };

    /*PhysicsObject& ScenePhysics::AddPhysicsObject(PhysicsObject& PhyObj)
    {
        PhyObj.


    };*/

}
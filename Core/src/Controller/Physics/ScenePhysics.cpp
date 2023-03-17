#include <Controller/Physics/ScenePhysics.hpp>

namespace Vakol::Controller::Physics 
{
    ScenePhysics::ScenePhysics(rp3d::PhysicsWorld*& newWorld) : m_world(newWorld){};

    void ScenePhysics::Init(){};

    void ScenePhysics::Update(const Time& time){};

    void ScenePhysics::SetGrav(){};

    void ScenePhysics::GetGrav(){};





}
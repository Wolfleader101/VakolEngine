#include "ECS/Components.hpp"

#include "SceneManager/Scene.hpp"

namespace Vakol::Components
{
    Transform::Transform(const Math::Vec3& pos, const Math::Quat& rot, const Math::Vec3& scale)
        : pos(pos), rot(rot), scale(scale)
    {
    }

    FSM::FSM(LuaTable table) : states(table)
    {
    }

    void FSM::AddState(const std::string& stateName, LuaFunction& callback)
    {
        // Add a new state to the states table
        states[stateName] = callback;
    }

    void FSM::ChangeState(const std::string& stateName)
    {
        // Change the current state
        currentState = stateName;
    }

    std::string FSM::GetState()
    {
        // Get the current state
        return currentState;
    }

    void FSM::Update()
    {
        // Call the callback for the current state
        const LuaFunction callback = states[currentState];
        callback();
    }

    TagType::TagType(uint8_t type) : type(static_cast<ENTITY_TYPE>(type)){};

    Tag::Tag(std::string& tag) : tag(std::move(tag)){};
} // namespace Vakol::Components

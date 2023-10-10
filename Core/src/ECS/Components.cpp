#include "ECS/Components.hpp"

#include "SceneManager/Scene.hpp"

namespace Vakol::Components
{
    Transform::Transform(const Math::Vec3& pos, const Math::Quat& rot, const Math::Vec3& scale)
        : pos(pos), rot(rot), scale(scale)
    {
    }

    void Transform::Update()
    {
        auto transform_matrix = Math::Mat4(1.0f);

        transform_matrix = Math::Translate(transform_matrix, pos);

        transform_matrix *= Math::Mat4Cast(rot);

        transform_matrix = Math::Scale(transform_matrix, scale);

        worldMatrix = transform_matrix;
    }

    const Math::Mat4& Transform::GetWorldMatrix() const
    {
        return worldMatrix;
    }

    const Math::Mat4& Transform::GetModelMatrix() const
    {
        return worldMatrix;
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

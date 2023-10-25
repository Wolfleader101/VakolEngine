#include "ECS/Components.hpp"

#include "SceneManager/Scene.hpp"

namespace Vakol::Components
{
    Transform::Transform(const Math::Vec3& pos, const Math::Quat& rot, const Math::Vec3& scale)
        : pos(pos), rot(rot), scale(scale)
    {
    }

    TagType::TagType(uint8_t type) : type(static_cast<ENTITY_TYPE>(type)){};

    Tag::Tag(std::string& tag) : tag(std::move(tag)){};
} // namespace Vakol::Components

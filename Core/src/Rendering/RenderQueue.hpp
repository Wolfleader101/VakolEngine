#pragma once

#include <entt/entt.hpp>

namespace Vakol::Rendering
{
    class RenderCommand;

    class RenderQueue
    {
    public:
        RenderCommand CreateCommand();

        void Clear();

        [[nodiscard]] bool Empty() const;

    private:
        entt::registry m_registry;

        friend class RenderCommand;
    };
}


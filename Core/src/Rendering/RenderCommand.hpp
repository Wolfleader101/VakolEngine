#pragma once

#include "RenderQueue.hpp"

namespace Vakol::Rendering
{
    struct VertexCommand
    {
        int nVertices = 0;
        int nIndices = 0;

        unsigned int vertexArray = 0u;
        unsigned int vertexBuffer = 0u;
    };

    struct ShaderCommand
    {
        unsigned int program = 0u; // the shader program id
    };

    struct TextureCommand
    {
        unsigned int texture = 0u; // the texture id
    };

    class RenderCommand
    {
    public:
        template <class T, typename... Args>
        void AddCommand(Args&&... args);

        template <typename... Types>
        bool HasCommand() const;

        template <class... T>
        decltype(auto) GetCommand() const;

    private:
        entt::entity m_handle = entt::null;
        RenderQueue* m_queue = nullptr;
    };

    template <class T, typename... Args>
    void RenderCommand::AddCommand(Args&&... args)
    {
        
    }

}

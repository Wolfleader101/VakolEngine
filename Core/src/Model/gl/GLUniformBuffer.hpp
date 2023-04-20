#pragma once

#include <Model/UniformBuffer.hpp>

namespace Vakol::Model::Assets
{
    class GLUniformBuffer : public UniformBuffer
    {   
    public:
        GLUniformBuffer(const int size, const int binding);
        ~GLUniformBuffer();

        void Bind() const override;
        void Unbind() const override;

        void SetData(const int offset, const int size, const void* data) const override;
    };
}

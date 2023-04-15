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

        void SetData(const void* data, const int size, const int offset) override;
    };
}

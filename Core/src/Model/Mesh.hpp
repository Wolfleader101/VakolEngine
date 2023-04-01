#pragma once

namespace Vakol::Model
{
    class Mesh
    {
    public:
        ~Mesh() = default;

        virtual void Draw(const unsigned int mode) const = 0;
        const virtual unsigned int GetID() const = 0;
    };
}

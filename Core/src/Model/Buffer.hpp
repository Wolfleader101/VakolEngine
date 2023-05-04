#pragma once

namespace Vakol::Model
{
    struct Buffer
    {
        Buffer() = default;
        Buffer(const unsigned int type, const int size, const unsigned int binding, const void* data, const unsigned int usage); // go ahead and set the data
        Buffer(const unsigned int type, const int size, const unsigned int binding, const unsigned int usage); // set data empty for now... 
        ~Buffer();

        void BindBufferBase(const unsigned int type, const unsigned int binding) const;
        void BindBufferRange(const unsigned int type, const unsigned int binding, const int offset, const int size) const;

        void GenBuffer(const int n); // generate a buffer id based on 'n'- amount of buffers
        
        void Bind(const unsigned int type) const;
        void Unbind(const unsigned int type) const;

        void SetData(const int size, const void* data, const unsigned int usage) const;
        void SetSubData(const int offset, const int size, const void* data) const;

        inline const unsigned int GetID() const { return ID; }
    private:
        unsigned int ID = 0;
    };
}
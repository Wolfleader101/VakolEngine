#pragma once

struct Buffer
{
    Buffer() = default;
    ~Buffer();

    void GenBuffer(const int n); // generate a buffer id based on 'n'- amount of buffers
    void BindBuffer(const unsigned int type) const; // assign a buffer slot based on it's uID

    void SetData(const unsigned int type, const int size, const void* data, const unsigned int usage) const;

    const unsigned int GetID() const { return ID; }
private:
    unsigned int ID;
};
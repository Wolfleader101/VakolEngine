#include "Utils/GUID.hpp"

namespace Vakol
{
    GUID::GUID()
    {
        id = xg::newGuid();
    }

    void GUID::GenNewGUID()
    {
        id = xg::newGuid();
    }

    std::string GUID::ConvertToString() const
    {
        return id.str();
    }

    bool GUID::IsValid() const
    {
        return id.isValid();
    }

    bool GUID::operator==(const GUID& other) const
    {
        return id == other.id;
    }

    bool GUID::operator!=(const GUID& other) const
    {
        return id != other.id;
    }

    bool GUID::operator<(const GUID& other) const
    {
        return id < other.id;
    }
} // namespace Vakol
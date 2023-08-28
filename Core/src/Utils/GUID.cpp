#include "Utils/GUID.hpp"

using namespace Vakol;

namespace Vakol
{
    void GUID::GenNewGUID()
    {
        id = xg::newGuid();
    }

    xg::Guid GUID::GetGUID() const
    {
        return id;
    }

    std::string GUID::ConvertToString() const
    {
        return id.str(); 
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
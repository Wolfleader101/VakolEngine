#include "Serializable.hpp"

namespace Vakol::Controller
{
    void Serializable::Serialize(const std::string &file) const { value->Serialize(file); };
    void Serializable::Deserialize(const std::string &file) { value->Deserialize(file); };
}  // namespace Vakol::Controller
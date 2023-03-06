#include "Serializable.hpp"

namespace Vakol::Controller
{
    void Serializable::OpenFile(const std::string& file, std::fstream& stream, std::fstream::open_mode Mode)
    {
        stream.open(file, Mode);
    }

    void Serializable::CloseFile(std::fstream& stream) 
    { 
        stream.close(); 
    }

    bool Serializable::StreamCheck(const std::fstream& stream) 
    { 
        return stream.good();
    }
}  // namespace Vakol::Controller
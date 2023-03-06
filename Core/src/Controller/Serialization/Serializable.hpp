#pragma once

#include <string>
#include <fstream>

namespace Vakol::Controller
{
	class Serializable
	{
       public:
        virtual void Serialize(const std::string& file) = 0;
        virtual void Deserialize(const std::string& file) = 0;

	   private:
		    //Kind of unnecessary but follows the "facade everything" mindset
		    void OpenFile(const std::string& file, std::fstream& stream, std::fstream::open_mode Mode);
            void CloseFile(std::fstream& stream);
            bool StreamCheck(std::fstream& stream);
	};
}
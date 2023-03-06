#pragma once 

#include <Controller/Serialization/Serializable>

namespace Vakol::Controller
{
	template <typename T, typename ... Args>
	class JSON : public Serializable
	{
       public:
            void Serialize(const std::string& file);
			void Deserialize(const std::string& file);
	};

}
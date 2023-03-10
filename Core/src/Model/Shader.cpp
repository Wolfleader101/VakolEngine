#include <fstream>
#include <Controller/Logger.hpp>

#include "Shader.hpp"

namespace Vakol::Model
{
	std::string Shader::ReadFile(const std::string& path)
	{
		std::string result;

		std::ifstream in(path, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII

		if (in)
		{
			in.seekg(0, std::ios::end);

			size_t size = in.tellg();

			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
				VK_ERROR("Could not read file: {0}", path);
		}
		else
			VK_ERROR("Could not open file: {0}", path);

		return result;
	}
}
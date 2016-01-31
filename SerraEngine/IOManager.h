#pragma once
#include <vector>
#include <fstream>

namespace SerraEngine
{
	class IOManager
	{
	public:
		static bool loadFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
		static bool loadFileToBuffer(std::string filePath, std::string &buffer);
	};
}


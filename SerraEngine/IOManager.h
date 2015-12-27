#pragma once
#include <vector>
#include <fstream>

namespace SerraEngine {
	class IOManager
	{
	public:
		static bool loadFileToBuffer(std::string filePath, std::vector<unsigned char> &buffer) {
			std::ifstream file(filePath, std::ios::binary);
			if (file.fail()) {
				perror(filePath.c_str());
				return false;
			}
			file.seekg(0, std::ios::end);
			int fileSize = (int)file.tellg();
			file.seekg(0, std::ios::beg);
			fileSize -= (int)file.tellg();//reduce file size by possible header bytes
			buffer.resize(fileSize);
			file.read((char*)&(buffer[0]), fileSize);
			file.close();
			return true;
		};
	};
}


#pragma once
#include <string>
#include "GLTexture.h"

namespace SerraEngine {
	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};
}


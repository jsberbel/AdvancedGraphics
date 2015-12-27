#pragma once
#include "TextureCache.h"
#include <string>

namespace SerraEngine {
	class ResourceManager
	{
		static TextureCache _textureCache;
	public:
		static GLTexture getTexture(std::string texturePath);
	};
}
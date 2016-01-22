#pragma once
#include "TextureCache.h"

namespace SerraEngine {
	class ResourceManager
	{
		static TextureCache m_textureCache;
	public:
		static GLTexture getTexture(std::string texturePath);
	};
}
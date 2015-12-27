#pragma once
#include <map>
#include "GLTexture.h"

namespace SerraEngine {
	class TextureCache
	{
		std::map<std::string, GLTexture> _textureMap;
	public:
		TextureCache();
		~TextureCache();
		GLTexture getTexture(std::string texturePath);
	};
}

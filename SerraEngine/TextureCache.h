#pragma once
#include <map>
#include "GLTexture.h"

namespace SerraEngine {
	class TextureCache
	{
		std::map<std::string, GLTexture> _textureMap;
	public:
		TextureCache() = default;
		~TextureCache() = default;
		GLTexture getTexture(std::string texturePath);
	};
}

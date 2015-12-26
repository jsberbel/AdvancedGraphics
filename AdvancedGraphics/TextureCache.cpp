#include <iostream>
#include "TextureCache.h"
#include "ImageLoader.h"

TextureCache::TextureCache()
{
}

TextureCache::~TextureCache()
{
}

GLTexture TextureCache::getTexture(std::string texturePath) {
	//look up the texture and check if is in the map
	auto mit = _textureMap.find(texturePath);
	//if its not inside the map
	if (mit == _textureMap.end()) {
		//load png into new GLTexture
		GLTexture newTexture = ImageLoader::loadPNG(texturePath);
		//insert it in the map
		//std::pair<std::string, GLTexture> newPair(texturePath, newTexture);
		_textureMap.insert(std::make_pair(texturePath, newTexture));
		std::cout << "New cache texture loaded" << std::endl;
		return newTexture;
	}
	std::cout << "Texture loaded" << std::endl;
	return mit->second;
}

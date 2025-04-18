#include "GlobalFunctions.h"

sf::Texture LoadTexture(const std::string& _filePath)
{
	sf::Texture texture;
	if (!texture.loadFromFile(_filePath))
	{
		std::cerr << "ERROR LOADING TEXTURE: " << _filePath << std::endl;
	}

	return texture;
}

#include "TextureManager.h"

sf::Texture* TextureManager::LoadTexture(const std::string& _filePath)
{
    sf::Texture* tempTexture = new sf::Texture();
    if (!tempTexture->loadFromFile(_filePath))
    {
        std::cerr << "ERROR LOADING TEXTURE: " << _filePath << std::endl;
        return nullptr;
    }

    return tempTexture;
}

void TextureManager::LoadTextures()
{
    textureSelected = LoadTexture(WHITE_BOX);
    textureNotSelected = LoadTexture(GRAY_BOX);
}
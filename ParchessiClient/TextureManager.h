#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#define TEXTURE_MANAGER TextureManager::Instance()

const std::string WHITE_BOX = "../Assets/Spritesheets/whiteBox.png";
const std::string GRAY_BOX = "../Assets/Spritesheets/grayBox.png";
const std::string TABLE_TEXTURE = "../Assets/Spritesheets/ParchisTable.png";

class TextureManager
{
private:

    sf::Texture* textureSelected;
    sf::Texture* textureNotSelected;

    TextureManager() = default;
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator =(const TextureManager&) = delete;


public:

    inline static TextureManager& Instance()
    {
        static TextureManager manager;
        return manager;
    }

    void LoadTextures(); 
    sf::Texture* LoadTexture(const std::string& _filePath);

    inline sf::Texture* GetSelected() const { return textureSelected; }
    inline sf::Texture* GetNotSelected() const { return textureNotSelected; }
};

#include "pch.h"
#include "ResourceManager.h"

namespace Battleship
{ 
	ResourceManager::ResourceManager() : disposed(false) {}

	ResourceManager::~ResourceManager()
	{
		for (auto it : this->music)
		{
			delete &it.second;
		}
		this->music.clear();
		disposed = true;
	}

	void ResourceManager::loadTextureFromFile(std::string textureTag, std::string filename)
	{
		sf::Texture texture;
		if(texture.loadFromFile(filename))
		{
			this->textures[textureTag] = texture;
		}
		else
		{
			std::cerr << "Could not load texture: " << textureTag << '\n';
		}
	}

	void ResourceManager::disposeTexture(std::string textureTag)
	{
		if (!disposed)
		{
			this->textures.erase(textureTag);
		}
	}

	sf::Texture& ResourceManager::getTexture(std::string textureTag)
	{
		return this->textures.at(textureTag);
	}

	void ResourceManager::loadFontFromFile(std::string fontTag, std::string filename)
	{
		sf::Font font;
		if (font.loadFromFile(filename))
		{
			this->fonts[fontTag] = font;
		}
		else
		{
			std::cerr << "Could not load font: " << fontTag << '\n';
		}
	}

	void ResourceManager::disposeFont(std::string fontTag)
	{
		if (!disposed)
		{
			this->textures.erase(fontTag);
		}
	}

	sf::Font& ResourceManager::getFont(std::string fontTag)
	{
		return this->fonts.at(fontTag);
	}

	void ResourceManager::loadMusicFromFile(std::string musicTag, std::string filename)
	{
		sf::Music *music = new sf::Music;
		if (music->openFromFile(filename))
		{
			this->music.emplace(musicTag, *music);
		}
		else
		{
			std::cerr << "Could not load font: " << musicTag << '\n';
		}
	}

	void ResourceManager::disposeMusic(std::string musicTag)
	{
		if (!disposed)
		{
			delete &(this->music.at(musicTag));
			this->music.erase(musicTag);
		}
	}

	sf::Music& ResourceManager::getMusic(std::string musicTag)
	{
		return this->music.at(musicTag);
	}

	void ResourceManager::resizeSprite(sf::Sprite& sprite, sf::Vector2f newBounds)
	{
		sprite.setScale(newBounds.x / sprite.getLocalBounds().width, newBounds.y / sprite.getLocalBounds().height);
	}
}
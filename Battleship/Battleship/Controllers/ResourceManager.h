#pragma once

#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace Battleship
{
	class ResourceManager
	{
	private:
		std::map<std::string, sf::Texture> textures;
		std::map<std::string, sf::Font> fonts;
		std::map<std::string, sf::Music&> music;

		bool disposed;
	public:
		ResourceManager();
		~ResourceManager();

		void loadTextureFromFile(std::string textureTag, std::string filename);
		void disposeTexture(std::string textureTag);
		sf::Texture& getTexture(std::string textureTag);

		void loadFontFromFile(std::string fontTag, std::string filename);
		void disposeFont(std::string fontTag);
		sf::Font& getFont(std::string fontTag);

		void loadMusicFromFile(std::string musicTag, std::string filename);
		void disposeMusic(std::string musicTag);
		sf::Music& getMusic(std::string musicTag);

		static void resizeSprite(sf::Sprite& sprite, sf::Vector2f newBounds);
	};
}
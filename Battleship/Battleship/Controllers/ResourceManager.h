#pragma once

//#define CONNECTION_STRING "dbname = BattleshipBase user = postgres password = 555 hostaddr = localhost port = 5432";
#include <pqxx/pqxx>
namespace Battleship
{
	class ResourceManager
	{
	private:
		std::map<std::string, sf::Texture> textures;
		std::map<std::string, sf::Font> fonts;
		std::map<std::string, sf::Music&> music;

		pqxx::connection* DatabaseConnection;
		
		bool connected;
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

		bool connectToDataBase(std::string connectionString);
		bool executeTransaction(std::string querry, pqxx::result& res, std::string& errorMsg);
		bool executeNonTransaction(std::string querry, pqxx::result& res, std::string& errorMsg);
		bool isConnectedToDatabase();
	};
}
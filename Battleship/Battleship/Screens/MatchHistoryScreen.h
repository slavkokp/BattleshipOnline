#pragma once
#include "../Controllers/ScreenManager.h"
#include "../CustomShapes/MatchHistoryLine.h"

namespace Battleship
{
	class MatchHistoryScreen : public Screen
	{
	private:
		GameData* data;
		Button* backBtn;
		MatchHistoryLine* header;
		std::vector<MatchHistoryLine*> matchList;
		sf::Text errorMsg;
		bool errorOccured;

		sf::View defaultView;
		sf::View listView;
		//private methods

		void initBackBtn(sf::Vector2f& windowSize, sf::Font& font);
		void initMatchesList(sf::Vector2f& windowSize, sf::Font& font);
		void initErrorMsg(std::string errorString, sf::Font& font);
		void initViews(sf::Vector2f& windowSize);

		bool getAllMatches(pqxx::result& res);
	public:
		MatchHistoryScreen(GameData* data);
		~MatchHistoryScreen();

		void updateButtonsFunction();
		void updateButtonsVisual();
		void update() override;
		void handleEvents() override;
		void render() override;
	};
}
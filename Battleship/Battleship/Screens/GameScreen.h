#pragma once

#include "../Controllers/ScreenManager.h"
#include "../CustomShapes/MapShape.h"
namespace Battleship
{
	class GameScreen : public Screen
	{
	private:
		GameData* data;
		MapShape playerClone;
		MapShape opponentsDummy;
	public:
		GameScreen(GameData* data);
		~GameScreen();

		void initPlayerClone();
		void initOpponentsDummy();
		
		void update()override;
		void handleEvents()override;
		void render()override;
	};
}
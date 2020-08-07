#include "pch.h"
#include "MatchHistoryScreen.h"

namespace Battleship
{
	MatchHistoryScreen::MatchHistoryScreen(GameData* data) : data(data)
	{
		sf::Vector2f windowSize(this->data->window.getSize());
		sf::Font& font = this->data->resourceManager.getFont("ButtonFont");
		this->errorOccured = false;

		this->initMatchesList(windowSize, font);
		this->initBackBtn(windowSize, font);
		this->initViews(windowSize);
	}

	MatchHistoryScreen::~MatchHistoryScreen()
	{
		this->data->window.setView(this->defaultView);
		delete backBtn;
		for (int i = 0; i < matchList.size(); i++)
		{
			delete matchList[i];
		}
		if (header != nullptr)
		{
			delete header;
		}
	}

	void MatchHistoryScreen::initBackBtn(sf::Vector2f& windowSize, sf::Font& font)
	{
		this->backBtn = new Button(sf::Vector2f(windowSize.x / 5.f, windowSize.y / 5.f), "Back", font, 16, 22, sf::Color::Black);
	}

	void MatchHistoryScreen::initMatchesList(sf::Vector2f& windowSize, sf::Font& font)
	{
		pqxx::result res;
		if (getAllMatches(res))
		{
			sf::Vector2f size(windowSize.x / 3 * 2, 20);
			std::string playerName = this->data->player.getName();

			this->header = new MatchHistoryLine(sf::Vector2f((windowSize.x - size.x) / 2.f, windowSize.y / 3.5f), size, &res[0], font, playerName, true);
			
			if (res.size() > 0)
			{
				sf::FloatRect headerBounds = header->getGlobalBounds();
				this->matchList.push_back(new MatchHistoryLine(sf::Vector2f(headerBounds.left, headerBounds.top + headerBounds.height), size, &res[0], font, playerName, false));
			}
			for (int i = 1; i < 100; i++)
			{
				sf::FloatRect prevBounds = matchList[i - 1]->getGlobalBounds();
				this->matchList.push_back(new MatchHistoryLine(sf::Vector2f(prevBounds.left, prevBounds.height + prevBounds.top), size, &res[i%2], font, playerName, false));
			}
		}
	}

	void MatchHistoryScreen::initErrorMsg(std::string errorString, sf::Font& font)
	{
		this->errorMsg.setString(errorString);
		this->errorMsg.setFont(font);
		this->errorMsg.setCharacterSize(16);
		this->errorMsg.setFillColor(sf::Color::White);
		sf::Vector2f windowSize(this->data->window.getSize());
		this->errorMsg.setPosition((windowSize.x - errorMsg.getGlobalBounds().width) / 2.f, (windowSize.y - errorMsg.getGlobalBounds().height) / 2.f);
	}

	void MatchHistoryScreen::initViews(sf::Vector2f& windowSize)
	{
		this->defaultView = this->data->window.getDefaultView();
		sf::FloatRect headerBounds = this->header->getGlobalBounds();
		this->listView = sf::View(sf::FloatRect(headerBounds.left, headerBounds.top, headerBounds.width, windowSize.y - windowSize.y / 3.5f));
		this->listView.setViewport(sf::FloatRect(0.16f, 0.285f, 0.66f, 0.715f));
		//this->listView.setSize(this->data->window.getSize().x * 0.84f, this->data->window.getSize().y * 0.715f);
	}

	bool MatchHistoryScreen::getAllMatches(pqxx::result& res)
	{
		std::string querry =
			"SELECT winner, loser, TO_CHAR(date_time, \'dd/mm/yyyy HH24:MI\') as date_time " \
			"FROM game_match " \
			"WHERE winner = \'" + this->data->player.getName() + "\' OR loser = \'" + this->data->player.getName() + "\' " \
			"ORDER BY date_time DESC";
		std::string errorString;
		if(this->data->resourceManager.executeNonTransaction(querry, res, errorString))
		{
			for (int i = 0; i < res.size(); i++)
			{
				for (int j = 0; j < 3; j++)
				{
					std::cout << res[i][j] << " ";
				}
				std::cout << "\n";
			}
			return true;
		}
		errorOccured = true;
		initErrorMsg(errorString, this->data->resourceManager.getFont("ButtonFont"));
		return false;
	}

	void MatchHistoryScreen::updateButtonFunction()
	{
		if (this->backBtn->isPressed())
		{
			
			this->data->screenManager.removeScreen();
		}
	}

	void MatchHistoryScreen::updateButtonVisual()
	{
		this->backBtn->update(this->data->window.mapPixelToCoords(sf::Mouse::getPosition(this->data->window), this->defaultView));
	}

	void MatchHistoryScreen::update()
	{
		this->updateButtonVisual();
		if (this->clock.getElapsedTime().asSeconds() > 0.3f)
		{
			this->updateButtonFunction();
		}
	}

	void MatchHistoryScreen::handleEvents()
	{
		sf::Event sfEvent;
		while (this->data->window.pollEvent(sfEvent))
		{
			if (sfEvent.type == sf::Event::Closed)
			{
				this->data->window.close();
			}
			if (sfEvent.type == sf::Event::MouseWheelScrolled)
			{
				if (sfEvent.mouseWheelScroll.delta > 0)
				{
					sf::FloatRect bounds = header->getGlobalBounds();
					if (!(listView.getCenter().y - listView.getSize().y / 2 < bounds.top))
					{
						this->listView.move(sf::Vector2f(0, sfEvent.mouseWheelScroll.delta * -20));
					}
				}
				else
				{
					sf::FloatRect bounds = this->matchList[matchList.size() - 1]->getGlobalBounds();
					if (!(listView.getCenter().y + listView.getSize().y / 2 > bounds.top + bounds.height))
					{
						this->listView.move(sf::Vector2f(0, sfEvent.mouseWheelScroll.delta * -20));
					}
				}
			}
		}
	}

	void MatchHistoryScreen::render()
	{
		this->data->window.clear();
		this->data->window.setView(this->defaultView);
		this->data->window.draw(*backBtn);
		if (this->errorOccured)
		{
			this->data->window.draw(errorMsg);
		}
		this->data->window.setView(this->listView);
		this->data->window.draw(*header);

		for (auto& it : matchList)
		{
			this->data->window.draw(*it);
		}
		this->data->window.display();
	}
}
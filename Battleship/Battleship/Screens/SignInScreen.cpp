#include "pch.h"
#include "SignInScreen.h"

namespace Battleship
{
	SignInScreen::SignInScreen(GameData* data) : data(data)
	{
		sf::Vector2f windowSize(this->data->window.getSize());
		sf::Font& font(this->data->resourceManager.getFont("AllTextFont"));
		this->initInputs(windowSize, font);
		this->initLabels(windowSize, font);
		this->initButtons(windowSize, font);
		this->initErrorMsg(windowSize, font);
		this->error = false;
		this->multipleCallsProtection = false;
	}

	SignInScreen::~SignInScreen()
	{
		for (auto& it : buttons)
		{
			delete it.second;
		}
		for (auto& it : inputs)
		{
			delete it.second;
		}
	}

	void SignInScreen::initInputs(sf::Vector2f& windowSize, sf::Font& font)
	{
		std::string tags[]{ "Nickname", "Email", "Password" };
		sf::Vector2f size(130, 20);
		for (int i = 0; i < 3; i++)
		{
			this->inputs[tags[i]] = new InputField(size);
			this->inputs[tags[i]]->setFont(font);
			this->inputs[tags[i]]->setOutlineColor(sf::Color::Green);
			this->inputs[tags[i]]->setPosition(sf::Vector2f((windowSize.x - size.x) / 2.f, (windowSize.y - size.y) / 4.f + (size.y * 2.5f * i)));
		}
	}

	void SignInScreen::initLabels(sf::Vector2f& windowSize, sf::Font& font)
	{
		std::string labelsText[]{ "Nickname", "Email", "Password" };
		sf::FloatRect bounds;
		for (int i = 0; i < 3; i++)
		{
			bounds = this->inputs[labelsText[i]]->getGlobalBounds();
			this->labels[labelsText[i]].setString(labelsText[i]);
			this->labels[labelsText[i]].setFont(font);
			this->labels[labelsText[i]].setCharacterSize(16);
			this->labels[labelsText[i]].setFillColor(sf::Color::White);
			this->labels[labelsText[i]].setPosition(bounds.left, bounds.top - this->labels[labelsText[i]].getGlobalBounds().height - 2.f);
		}
	}

	void SignInScreen::initButtons(sf::Vector2f& windowSize, sf::Font& font)
	{
		sf::FloatRect passwordBounds = inputs["Password"]->getGlobalBounds();
		std::string btnText[]{ "Sign in", "Back" };
		for (int i = 0; i < 2; i++)
		{
			this->buttons[btnText[i]] = new Button(sf::Vector2f(0, 0), btnText[i], font, 16, 22, sf::Color::Black);
			this->buttons[btnText[i]]->move(sf::Vector2f((windowSize.x - this->buttons[btnText[i]]->getGlobalBounds().width) / 2,
				passwordBounds.top + passwordBounds.height * 2 * (i + 1)));
		}
	}

	void SignInScreen::initErrorMsg(sf::Vector2f& windowSize, sf::Font& font)
	{
		sf::FloatRect bounds = this->buttons["Back"]->getGlobalBounds();
		this->errorMsg.setString("Cannot connect to the database");
		this->errorMsg.setFont(font);
		this->errorMsg.setCharacterSize(16);
		this->errorMsg.setFillColor(sf::Color::White);
		this->errorMsg.setPosition((windowSize.x - errorMsg.getGlobalBounds().width) / 2.f, bounds.top + bounds.height * 2);
	}

	void SignInScreen::changeErrorMsgString(std::string str)
	{
		sf::FloatRect bounds = this->buttons["Back"]->getGlobalBounds();
		this->errorMsg.setString(str);
		this->errorMsg.setPosition((this->data->window.getSize().x - errorMsg.getGlobalBounds().width) / 2.f, bounds.top + bounds.height * 2);
	}

	bool SignInScreen::parseInput(std::map<std::string, std::string>& result, std::string& errorString)
	{
		result["Nickname"] = this->inputs["Nickname"]->getText();
		result["Password"] = this->inputs["Password"]->getText();
		result["Email"] = this->inputs["Email"]->getText();
		std::regex regExp("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
		if (!std::regex_match(result["Email"], regExp))
		{
			errorString = "Invalid Email";
			return false;
		}
		for (auto& it : result)
		{
			if (!it.second.length())
			{
				errorString = it.first + " field is empty!";
				return false;
			}
		}
		return true;
	}

	void SignInScreen::updateButtonsFunction()
	{
		if (buttons["Sign in"]->isPressed())
		{
			if (!this->multipleCallsProtection)
			{
				if (signIn())
				{
					this->data->screenManager.removeScreen();
				}
			}
		}
		if (buttons["Back"]->isPressed())
		{
			this->data->screenManager.removeScreen();
		}
	}

	bool SignInScreen::signIn()
	{
		this->multipleCallsProtection = true;
		if (this->data->resourceManager.isConnectedToDatabase())
		{
			std::string errorText;
			std::map<std::string, std::string> input;
			if (!this->parseInput(input, errorText))
			{
				this->error = true;
				this->changeErrorMsgString(errorText);
				return false;
			}
			std::string querry =
				"INSERT INTO Player " \
				"VALUES(" \
				"(SELECT MAX(player_id) " \
				"FROM player) + 1, " \
				"\'" + input["Nickname"] + "\', \'" + input["Password"] + "\', \'" + input["Email"] + "\');";
			
			pqxx::result res;
			if (!this->data->resourceManager.executeTransaction(querry, res, errorText))
			{
				this->error = true;
				this->changeErrorMsgString(errorText);
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			return false;
		}
	}

	void SignInScreen::updateButtonsVisual()
	{
		for (auto& it : buttons)
		{
			it.second->update(this->data->inputManager.getMousePosView());
		}
	}

	void SignInScreen::update()
	{
		this->data->inputManager.updateMousePosView(this->data->window);
		this->updateButtonsVisual();
		if (this->clock.getElapsedTime().asSeconds() > 0.3f)
		{
			this->updateButtonsFunction();
		}
	}

	void SignInScreen::handleEvents()
	{
		sf::Event sfEvent;
		while (this->data->window.pollEvent(sfEvent))
		{
			if (sfEvent.type == sf::Event::Closed)
			{
				this->data->window.close();
			}
			if (sfEvent.type == sf::Event::MouseButtonPressed)
			{
				for (auto& it : inputs)
				{
					it.second->update(this->data->inputManager.getMousePosView(), sfEvent.mouseButton.button);
				}
			}
			if (sfEvent.type == sf::Event::KeyPressed)
			{
				this->multipleCallsProtection = false;
				for (auto& it : inputs)
				{
					it.second->updateCopy(sfEvent);
					it.second->updatePaste(sfEvent);
				}
			}
			if (sfEvent.type == sf::Event::TextEntered)
			{
				// backspace || all glyphs
				if (sfEvent.text.unicode == 8 || (sfEvent.text.unicode >= 32 && sfEvent.text.unicode <= 126))
				{
					for (auto& it : inputs)
					{
						it.second->updateText(sfEvent.text.unicode);
					}
				}
			}
		}
	}

	void SignInScreen::render()
	{
		this->data->window.clear();
		for (auto& it : inputs)
		{
			this->data->window.draw(*it.second);
		}
		for (auto& it : buttons)
		{
			this->data->window.draw(*it.second);
		}
		for (auto& it : labels)
		{
			this->data->window.draw(it.second);
		}
		if (error)
		{
			this->data->window.draw(errorMsg);
		}
		this->data->window.display();
	}
}
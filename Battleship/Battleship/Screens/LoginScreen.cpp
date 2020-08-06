#include "pch.h"
#include "LoginScreen.h"

namespace Battleship
{
	LoginScreen::LoginScreen(GameData* data) : data(data)
	{
		sf::Vector2f windowSize(data->window.getSize());
		
		this->data->resourceManager.loadFontFromFile("AllTextFont", "Resourses/VINERITC.TTF");
		sf::Font& font = this->data->resourceManager.getFont("AllTextFont");

		this->rememberMe = false;

		this->initInputs(windowSize, font);
		this->initCheckbox(windowSize, font);
		this->initButtons(windowSize, font);
		this->initLabels(windowSize, font);
		this->initErrorMsg(windowSize, font);
		this->error = false;
		this->multipleCallsProtection = false;
	}

	LoginScreen::~LoginScreen()
	{
		for (auto& it : buttons)
		{
			delete it.second;
		}
		for (auto& it : inputs)
		{
			delete it.second;
		}
		this->data->resourceManager.disposeFont("AllTextFont");
	}

	void LoginScreen::initButtons(sf::Vector2f& windowSize, sf::Font& font)
	{
		sf::FloatRect bounds = checkboxText.getGlobalBounds();//inputs["Password"]->getGlobalBounds();
		std::string btnText[]{ "Log in", "Sign in" };
		for (int i = 0; i < 2; i++)
		{
			this->buttons[btnText[i]] = new Button(sf::Vector2f(0, 0), btnText[i], font, 16, 22, sf::Color::Black);
			this->buttons[btnText[i]]->move(sf::Vector2f((windowSize.x - this->buttons[btnText[i]]->getGlobalBounds().width) / 2,
				bounds.top + bounds.height * 2 * (i + 1)));
		}
	}

	void LoginScreen::initInputs(sf::Vector2f& windowSize, sf::Font& font)
	{
		sf::Vector2f size(130, 20);
		std::string inputName[]{ "Email", "Password" };
		for (int i = 0; i < 2; i++)
		{
			this->inputs[inputName[i]] = new InputField(size);
			this->inputs[inputName[i]]->setFont(font);
			this->inputs[inputName[i]]->setOutlineColor(sf::Color::Green);
			this->inputs[inputName[i]]->setPosition(sf::Vector2f((windowSize.x - size.x) / 2.f, (windowSize.y - size.y) / 3.f + (size.y * 2.5f * i)));
		}

		std::ifstream file;
		file.open("Utils/Remember_me.txt");
		if (!(file.peek() == std::ifstream::traits_type::eof()))
		{
			std::string str;
			file >> str;
			this->inputs["Email"]->setString(str);
			file >> str;
			this->inputs["Password"]->setString(str);
			this->rememberMe = true;
		}
		file.close();
	}

	void LoginScreen::initLabels(sf::Vector2f& windowSize, sf::Font& font)
	{
		std::string labelsText[]{ "Email", "Password" };
		sf::FloatRect bounds;
		for (int i = 0; i < 2; i++)
		{
			bounds = this->inputs[labelsText[i]]->getGlobalBounds();
			this->labels[labelsText[i]].setString(labelsText[i]);
			this->labels[labelsText[i]].setFont(font);
			this->labels[labelsText[i]].setCharacterSize(16);
			this->labels[labelsText[i]].setFillColor(sf::Color::White);
			this->labels[labelsText[i]].setPosition(bounds.left, bounds.top - this->labels[labelsText[i]].getGlobalBounds().height - 2.f);
		}
	}

	void LoginScreen::initErrorMsg(sf::Vector2f& windowSize, sf::Font& font)
	{
		sf::FloatRect bounds = this->buttons["Sign in"]->getGlobalBounds();
		this->errorMsg.setString("Wrong email or password");
		this->errorMsg.setFont(font);
		this->errorMsg.setCharacterSize(16);
		this->errorMsg.setFillColor(sf::Color::White);
		this->errorMsg.setPosition((windowSize.x - errorMsg.getGlobalBounds().width) / 2.f, bounds.top + bounds.height * 2);
	}

	void LoginScreen::initCheckbox(sf::Vector2f& windowSize, sf::Font& font)
	{
		this->rememberMe ? this->checkbox.setFillColor(sf::Color::Green) : this->checkbox.setFillColor(sf::Color::Red);
		this->checkbox.setOutlineThickness(-1.f);
		this->checkbox.setOutlineColor(sf::Color::White);
		this->checkbox.setSize(sf::Vector2f(20, 20));
		sf::FloatRect bounds = this->inputs["Password"]->getGlobalBounds();
		this->checkbox.setPosition(bounds.left, bounds.top + bounds.height * 2 + 1.f);

		this->checkboxText.setString("Remember me");
		this->checkboxText.setFont(font);
		this->checkboxText.setCharacterSize(16);
		this->checkboxText.setFillColor(sf::Color::White);
		bounds = checkbox.getGlobalBounds();
		this->checkboxText.setPosition(bounds.left + bounds.width + 2.f, bounds.top);
	}

	void LoginScreen::changeErrorMsgString(std::string str)
	{
		sf::FloatRect bounds = this->buttons["Sign in"]->getGlobalBounds();
		this->errorMsg.setString(str);
		this->errorMsg.setPosition((this->data->window.getSize().x - errorMsg.getGlobalBounds().width) / 2.f, bounds.top + bounds.height * 2);
	}

	void LoginScreen::editRememberMe()
	{
		std::ofstream file;
		file.open("Utils/Remember_me.txt");
		file.clear();
		if (rememberMe)
		{
			file << this->inputs["Email"]->getText() + "\n" + this->inputs["Password"]->getText();
		}
		file.close();
	}

	void LoginScreen::updateButtonsFunction()
	{
		if (buttons["Log in"]->isPressed())
		{
			if (!this->multipleCallsProtection)
			{
				pqxx::result r;
				if (this->login(r))
				{
					this->data->player.setName(r[0][0].c_str());
					this->data->screenManager.addScreen(new MenuScreen(data), true);
				}
			}
		}
		if (buttons["Sign in"]->isPressed())
		{
			this->data->screenManager.addScreen(new SignInScreen(data), false);
		}
	}

	void LoginScreen::updateButtonsVisual()
	{
		for (auto& it : buttons)
		{
			it.second->update(this->data->inputManager.getMousePosView());
		}
	}

	bool LoginScreen::login(pqxx::result& r)
	{
		this->multipleCallsProtection = true;
		std::string querry =
			"SELECT username " \
			"FROM Player " \
			"WHERE email = \'" + this->inputs["Email"]->getText() + "\' AND password = \'" + this->inputs["Password"]->getText() + "\'";
		std::string errorText;
		if (this->data->resourceManager.isConnectedToDatabase())
		{
			if (!this->data->resourceManager.executeNonTransaction(querry, r, errorText))
			{
				this->error = true;
				this->changeErrorMsgString(errorText);
				return false;
			}
			else
			{
				if (!r.size())
				{
					this->error = true;
					this->changeErrorMsgString("Wrong email or password");
					return false;
				}
				else
				{
					editRememberMe();
					return true;
				}
			}
		}
		else
		{
			this->error = true;
			this->changeErrorMsgString("Can't connect to database(try restarting)");
			return false;
		}
	}

	void LoginScreen::updateCheckbox(sf::Mouse::Button& btn)
	{
		if (this->checkbox.getGlobalBounds().contains(this->data->inputManager.getMousePosView()))
		{
			if (btn == sf::Mouse::Left)
			{
				this->rememberMe = !rememberMe;
				if (rememberMe)
				{
					this->checkbox.setFillColor(sf::Color::Green);
				}
				else
				{
					this->checkbox.setFillColor(sf::Color::Red);
				}
			}
		}
	}
	
	void LoginScreen::update()
	{
		this->data->inputManager.updateMousePosView(this->data->window);
		this->updateButtonsVisual();
		if (this->clock.getElapsedTime().asSeconds() > 0.3f)
		{
			this->updateButtonsFunction();
		}
	}

	void LoginScreen::handleEvents()
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
				this->updateCheckbox(sfEvent.mouseButton.button);
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

	void LoginScreen::render()
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
		this->data->window.draw(checkbox);
		this->data->window.draw(checkboxText);
		if (error)
		{
			this->data->window.draw(errorMsg);
		}
		this->data->window.display();
	}
}